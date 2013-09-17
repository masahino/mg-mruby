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
#include "mrb_keymap.h"
#include "mrb_hook.h"

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
mrb_mg_method_missing(mrb_state *mrb, mrb_value self)
{
     mrb_value name, *a;
     int alen, i;
     char *command_str, *tmp;
     mrb_get_args(mrb, "o*", &name, &a, &alen);
     
     command_str = strdup(RSTRING_PTR(mrb_sym2str(mrb, mrb_symbol(name))));
     tmp = command_str;
     while (*tmp) {
	  if (*tmp == '_') 
	       *tmp = '-';
	  tmp++;
     }
     for (i = 0; i < alen; i++) {
	  switch (mrb_type(a[i])) {
	  case MRB_TT_STRING:
	       command_str = realloc(command_str, 
				     strlen(command_str) +
				     RSTRING_LEN(a[i]) + 4);
	       strcat(command_str, " \"");
	       strcat(command_str, RSTRING_PTR(a[i]));
	       strcat(command_str, "\"");
	       break;
	  case MRB_TT_SYMBOL:
//	       mg_name = mrb_str_plus(mrb, mg_name, mrb_sym2str(mrb, mrb_symbol(a[i])));
//	       break;
	  default:
	       command_str = realloc(command_str,
				     strlen(command_str) +
				     RSTRING_LEN(mrb_obj_as_string(mrb, a[i])) + 2);
	       command_str = strcat(command_str, " ");
	       command_str = strcat(command_str,
				    RSTRING_PTR(mrb_obj_as_string(mrb, a[i])));
	       break;
	  }
     }
     return mrb_fixnum_value(excline(command_str));
     
}

mrb_value
mrb_mg_buffer_string(mrb_state *mrb, mrb_value self)
{
     return mrb_str_new(mrb, ltext(curwp->w_dotp), llength(curwp->w_dotp));
}

mrb_value
mrb_mg_current_position(mrb_state *mrb, mrb_value self)
{
     struct line *clp;
     int nline;

     mrb_value cursor_a;
     cursor_a = mrb_ary_new(mrb);

     clp = bfirstlp(curbp);
     nline = 0;
     for (;;) {
	  ++nline;
	  if (clp == curwp->w_dotp) {
	       break;
	  }
	  clp = lforw(clp);
	  if (clp == curbp->b_headp)
	       break;
     }
     mrb_ary_push(mrb, cursor_a, mrb_fixnum_value(nline));
     mrb_ary_push(mrb, cursor_a, mrb_fixnum_value(getcolpos()));
     return cursor_a;
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
		   ewprintf("%s", 
			    RSTRING_PTR(mrb_funcall(mrb, obj, "inspect", 0)));
		   return FALSE;
	      }
	  }
	  return TRUE;
     }
     return FALSE;
}

mrb_value
mrb_mg_load_file(mrb_state *mrb, mrb_value self)
{
     mrb_value fname;
     int ret;

     mrb_get_args(mrb, "S", &fname);
     ret = mrb_mg_load(RSTRING_PTR(fname));
     if (ret == TRUE)
	  return mrb_true_value();
     return mrb_false_value();
}

static mrb_value
mrb_mg_eval_string(mrb_state *mrb, const char *str, int len)
{
     mrbc_context* cxt;

     cxt = curbp->b_mrb_cxt;
     mrb->exc = 0;
     return mrb_load_nstring_cxt(mrb, str, len, cxt);
}

int
mrb_mg_eval_last_exp(int f, int n)
{
     mrb_value ret, ret_str;
     int ai;
     ai = mrb_gc_arena_save(mrb);
     ret = mrb_mg_eval_string(mrb,
			      ltext(curwp->w_dotp), llength(curwp->w_dotp));
     if (!mrb->exc) {
	  ret_str = mrb_funcall(mrb, ret, "to_s", 0);
     }
     if (mrb->exc) {
	  mrb_value obj;
	  obj = mrb_obj_value(mrb->exc);
	  ret_str = mrb_funcall(mrb, obj, "inspect", 0);
     }
     ewprintf("%s", (RSTRING_PTR(ret_str)));
     mrb_gc_arena_restore(mrb, ai);
     return TRUE;
}

int 
mrb_mg_eval_print_last_exp(int f, int n)
{
     mrb_value ret, ret_str;
     int s, ai;
     ai = mrb_gc_arena_save(mrb);
     ret = mrb_mg_eval_string(mrb,
			      ltext(curwp->w_dotp), llength(curwp->w_dotp));
     s = newline(FFRAND, 1);
     if (s != TRUE) {
	  mrb_gc_arena_restore(mrb, ai);
	  return FALSE;
     }
     if (!mrb->exc) {
	  ret_str = mrb_funcall(mrb, ret, "to_s", 0);
     }
     if (mrb->exc) {
	  mrb_value obj;
	  obj = mrb_obj_value(mrb->exc);
	  ret_str = mrb_funcall(mrb, obj, "inspect", 0);
     } 
     s = linsert_str(RSTRING_PTR(ret_str), RSTRING_LEN(ret_str));
     newline(FFRAND, 1);
     mrb_gc_arena_restore(mrb, ai);
     return TRUE;
}

int
mrb_mg_evalexpr(char *line)
{
     mrb_value ret, ret_str;
     int ai;
     ai = mrb_gc_arena_save(mrb);

     ret = mrb_mg_eval_string(mrb, line, strlen(line));
     if (!mrb->exc) {
	  ret_str = mrb_funcall(mrb, ret, "to_s", 0);
     }
     if (mrb->exc) {
	  mrb_value obj;
	  obj = mrb_obj_value(mrb->exc);
	  ret_str = mrb_funcall(mrb, obj, "inspect", 0);
     }
     ewprintf("%s", (RSTRING_PTR(ret_str)));
     mrb_gc_arena_restore(mrb, ai);
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
     mrb->exc = 0;
     ret = mrb_load_nstring(mrb, RSTRING_PTR(buf_str),
				RSTRING_LEN(buf_str));
     if (mrb->exc) {
	  mrb_value obj;
	  obj = mrb_obj_value(mrb->exc);
	  ewprintf("%s", RSTRING_PTR(mrb_funcall(mrb, obj, "inspect", 0)));
	  return FALSE;
     }
     return TRUE;
}

int
mrb_mg_eval_region(int f, int n)
{
     struct region reg;
     struct line *flp, *blp;
     long fsize, bsize;
     char *text;
     mrb_value ret;
     
     if (curwp->w_markp == NULL) {
	  ewprintf("No mark set int this window");
	  return FALSE;
     }

     /* "r_size" always ok */
     if (curwp->w_dotp == curwp->w_markp) {
	  reg.r_linep = curwp->w_dotp;
	  reg.r_lineno = curwp->w_dotline;
	  if (curwp->w_doto < curwp->w_marko) {
	       reg.r_offset = curwp->w_doto;
	       reg.r_size = (RSIZE)(curwp->w_marko - curwp->w_doto);
	  } else {
	       reg.r_offset = curwp->w_marko;
	       reg.r_size = (RSIZE)(curwp->w_doto - curwp->w_marko);
	  }
     } else {
	  /* get region size */
	  flp = blp = curwp->w_dotp;
	  bsize = curwp->w_doto;
	  fsize = llength(flp) - curwp->w_doto + 1;
	  while (lforw(flp) != curbp->b_headp || lback(blp) != curbp->b_headp) {
	       if (lforw(flp) != curbp->b_headp) {
		    flp = lforw(flp);
		    if (flp == curwp->w_markp) {
			 reg.r_linep = curwp->w_dotp;
			 reg.r_offset = curwp-> w_doto;
			 reg.r_lineno = curwp->w_dotline;
			 reg.r_size = fsize + curwp->w_marko;
			 break;
		    }
		    fsize += llength(flp) + 1;
	       }
	       if (lback(blp) != curbp->b_headp) {
		    blp = lback(blp);
		    bsize += llength(blp) + 1;
		    if (blp == curwp->w_markp) {
			 reg.r_linep = blp;
			 reg.r_offset = curwp->w_marko;
			 reg.r_lineno = curwp->w_markline;
			 reg.r_size = bsize - curwp->w_marko;
			 break;
		    }
	       }
	  }
     }
     if ((text = malloc(reg.r_size + 1)) == NULL)
	  return ABORT;
     region_get_data(&reg, text, reg.r_size);
	  
     ret = mrb_load_string(mrb, text);
     if (mrb->exc) {
	  mrb_value obj;
	  obj = mrb_obj_value(mrb->exc);
	  ewprintf("%s", RSTRING_PTR(mrb_funcall(mrb, obj, "inspect", 0)));
	  return FALSE;
     }
     return TRUE;
	       
}

mrb_value
mrb_mg_set_fileencodings(mrb_state *mrb, mrb_value self)
{
    mrb_value fileencodings;

    mrb_get_args(mrb, "A", &fileencodings);

    mrb_cv_set(mrb, self, mrb_intern(mrb, "@@fileencodings"), fileencodings);

    return mrb_nil_value();
}

mrb_value
mrb_mg_get_fileencodings(mrb_state *mrb, mrb_value self)
{
    mrb_value fileencodings;

    fileencodings = mrb_cv_get(mrb, self, mrb_intern(mrb, "@@fileencodings"));

    return fileencodings;
}

void
mrb_mg_init()
{
     struct RClass *mg;

     mrb = mrb_open();

    mg = mrb_define_module(mrb, "MG");

    mrb_define_module_function(mrb, mg, "debug_log", 
			       mrb_mg_debug_log, ARGS_REQ(1));

    mrb_define_module_function(mrb, mg, "method_missing",
			       mrb_mg_method_missing, ARGS_ANY());
    /* buffer_string */
    mrb_define_module_function(mrb, mg, "buffer_string",
			       mrb_mg_buffer_string, ARGS_NONE());

    /* current_position */
    mrb_define_module_function(mrb, mg, "current_position",
			       mrb_mg_current_position, ARGS_NONE());

    mrb_define_module_function(mrb, mg, "load",
			       mrb_mg_load_file, ARGS_REQ(1));

    mrb_define_module_function(mrb, mg, "fileencodings=", mrb_mg_set_fileencodings, ARGS_REQ(1));

    mrb_define_module_function(mrb, mg, "get_fileencodings", mrb_mg_get_fileencodings, ARGS_NONE());
    /* const */
    mrb_define_const(mrb, mg, "FFRAND", mrb_fixnum_value(FFRAND));

    mrb_mod_cv_set(mrb, mg, mrb_intern(mrb, "@@fileencodings"), mrb_ary_new(mrb));

    mrb_mode_init(mrb);
    mrb_extend_init(mrb);
    mrb_autoexec_init(mrb);
    mrb_buffer_init(mrb);
    mrb_keymap_init(mrb);
    mrb_command_init(mrb);
    mrb_hook_init(mrb);

    funmap_add(mrb_mg_eval_last_exp, "eval-last-exp");
    funmap_add(mrb_mg_eval_print_last_exp, "eval-print-last-exp");
    funmap_add(mrb_mg_eval_region, "eval-region");
}

//#endif
