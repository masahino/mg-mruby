/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "def.h"
#include "funmap.h"
#include "kbd.h"

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/class.h"
#include "mruby/compile.h"
#include "mruby/dump.h"

#include "mrb_mg.h"
#include "mrb_mode.h"
#include "mrb_extend.h"
#include "mrb_autoexec.h"
#include "mrb_buffer.h"

mrb_state *mrb;


mrb_value
mrb_mg_debug_log(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     char *cstr;
     mrb_get_args(mrb, "S", &value);
     cstr = strndup(RSTRING_PTR(value), RSTRING_LEN(value));
     fprintf(stderr, "[debug]%s\n", cstr);
     return self;
}

mrb_value
mrb_mg_extend(mrb_state *mrb, mrb_value self)
{
    mrb_value extend_str;
    mrb_get_args(mrb, "S", &extend_str);
    return mrb_fixnum_value(excline(RSTRING_PTR(extend_str)));
}

mrb_value
mrb_mg_method_missing(mrb_state *mrb, mrb_value self)
{
     mrb_value name, *a, mg_name;
     int alen, i;

     char *command_str;
     mrb_get_args(mrb, "o*", &name, &a, &alen);
     
     mg_name = mrb_sym2str(mrb, mrb_symbol(name));
     mg_name = mrb_funcall(mrb, mg_name, "gsub", 2, 
			   mrb_str_new_cstr(mrb, "_"), 
			   mrb_str_new_cstr(mrb, "-"));
     for (i = 0; i < alen; i++) {
	  mg_name = mrb_str_plus(mrb, mg_name, mrb_str_new_cstr(mrb, " "));
	  switch (mrb_type(a[i])) {
	  case MRB_TT_STRING:
	       mg_name = mrb_str_plus(mrb, mg_name, mrb_str_new_cstr(mrb, "\""));
	       mg_name = mrb_str_plus(mrb, mg_name, a[i]);
	       mg_name = mrb_str_plus(mrb, mg_name, mrb_str_new_cstr(mrb, "\""));
	       break;
	  case MRB_TT_SYMBOL:
//	       mg_name = mrb_str_plus(mrb, mg_name, mrb_sym2str(mrb, mrb_symbol(a[i])));
//	       break;
	  default:
	       mg_name = mrb_str_plus(mrb, mg_name, mrb_obj_as_string(mrb, a[i]));
	       break;
	  }
     }
     command_str = strdup(RSTRING_PTR(mg_name));
     fprintf(stderr, "[%s]\n", command_str);
     return mrb_fixnum_value(excline(command_str));
     
}

mrb_value
mrb_mg_buffer_string(mrb_state *mrb, mrb_value self)
{
     return mrb_str_new(mrb, ltext(curwp->w_dotp), llength(curwp->w_dotp));
}

mrb_value
mrb_mg_dired_backup_unflag(mrb_state *mrb, mrb_value self)
{
     PF func;
     mrb_value value;
     mrb_get_args(mrb, "o", &value);
     func = name_function("dired-backup-unflag");
     (*func)(0, mrb_type(value));
     return mrb_nil_value();
}


int
mrb_mg_load(char *fname)
{
     FILE *f;
     mrb_value v;
     
     if ((fname = adjustname(fname, TRUE)) == NULL) {
	  /* just to be careful */
	  return (FALSE);
     }
     f = fopen(fname, "r");
     if (f != NULL) {
	  mrb->exc = 0;
	  if (strlen(fname) > 4 &&
	      strcmp(&fname[strlen(fname)-4], ".mrb") == 0) {
	       v = mrb_load_irep_file(mrb, f);
	  } else {
	       v = mrb_load_file(mrb, f);
	  }
	  fclose(f);
	  if (mrb->exc) {
              if (!mrb_undef_p(v)) {
		       mrb_value obj;
		       obj = mrb_obj_value(mrb->exc);
//		       obj2 = mrb_funcall(mrb, obj, "inspect", 0);
		printf("%s\n", RSTRING_PTR(mrb_any_to_s(mrb, obj)));
                  return FALSE;
	      }
	  }
	  return TRUE;
     }
     return FALSE;
}

static mrb_value
mrb_mg_eval_string(mrb_state *mrb, const char *str, int len)
{
    mrb_value ret, ret_str;
    mrb->exc = 0;
    ret = mrb_load_nstring(mrb, str, len);
    ret_str = mrb_funcall(mrb, ret, "to_s", 0);
    return ret_str;
}

int
mrb_mg_eval_last_exp(int f, int n)
{
    mrb_value ret;
    ret = mrb_mg_eval_string(mrb, ltext(curwp->w_dotp), llength(curwp->w_dotp));
    if (mrb->exc) {
	 mrb_value obj;
	 obj = mrb_obj_value(mrb->exc);
	 ewprintf("%s\n", RSTRING_PTR(mrb_any_to_s(mrb, obj)));
	 return FALSE;
    }
    ewprintf("%s", (RSTRING_PTR(ret)));
    return TRUE;
}

int 
mrb_mg_eval_print_last_exp(int f, int n)
{
    mrb_value eval_result;
    int s;
    eval_result = mrb_mg_eval_string(mrb, ltext(curwp->w_dotp), llength(curwp->w_dotp));
    s = newline(FFRAND, 1);
    if (s != TRUE) {
        return FALSE;
    }
    s = linsert_str(RSTRING_PTR(eval_result), RSTRING_LEN(eval_result));
    newline(FFRAND, 1);
    return TRUE;
}

int
mrb_mg_evalbuffer(int f, int n)
{
     struct line *lp;
     struct buffer *bp = curbp;
     mrb_value buf_str, ret;

     buf_str = mrb_str_new_cstr(mrb, "");

     for (lp = bfirstlp(bp); lp != bp->b_headp; lp = lforw(lp)) {
	  buf_str = mrb_str_cat(mrb, buf_str, ltext(lp), llength(lp));
	  buf_str = mrb_str_cat2(mrb, buf_str, "\n");
     }
     ret = mrb_load_string(mrb, RSTRING_PTR(buf_str));
     return TRUE;
}


/* match.c */
mrb_value
mrb_mg_showmatch(mrb_state *mrb, mrb_value self)
{
     mrb_value f, n;
     int ret;
     mrb_get_args(mrb, "oo", &f, &n);
     ret = showmatch(mrb_fixnum(f), mrb_fixnum(n));
     return mrb_fixnum_value(ret);
}

/* random.c */
mrb_value
mrb_mg_indent(mrb_state *mrb, mrb_value self)
{
     mrb_value n;
     int ret;

     mrb_get_args(mrb, "o", &n);
     ret = indent(FFOTHARG, mrb_fixnum(n));
     return mrb_fixnum_value(ret);
}

void
mrb_mg_init()
{
     struct RClass *mg;

     mrb = mrb_open();

    mg = mrb_define_module(mrb, "MG");

    mrb_define_module_function(mrb, mg, "dired_backup_unflag=", 
			       mrb_mg_dired_backup_unflag, ARGS_REQ(1));

    mrb_define_module_function(mrb, mg, "debug_log", 
			       mrb_mg_debug_log, ARGS_REQ(1));

//    mrb_define_module_function(mrb, mg, "extend",
//                               mrb_mg_extend, ARGS_REQ(1));

    mrb_define_module_function(mrb, mg, "method_missing",
			       mrb_mg_method_missing, ARGS_ANY());
    /* buffer_string */
    mrb_define_module_function(mrb, mg, "buffer_string",
			       mrb_mg_buffer_string, ARGS_NONE());

    /* match.c */
    mrb_define_module_function(mrb, mg, "showmatch",
			       mrb_mg_showmatch, ARGS_REQ(2));

    /* random.c */
    mrb_define_module_function(mrb, mg, "indent",
			       mrb_mg_indent, ARGS_REQ(1));

    /* const */
    mrb_define_const(mrb, mg, "FFRAND", mrb_fixnum_value(FFRAND));


    mrb_mode_init(mrb);
    mrb_extend_init(mrb);
    mrb_autoexec_init(mrb);
    mrb_buffer_init(mrb);

    funmap_add(mrb_mg_eval_last_exp, "eval-last-exp");
    funmap_add(mrb_mg_eval_print_last_exp, "eval-print-last-exp");
}

//#endif
