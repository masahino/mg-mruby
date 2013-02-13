/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "estruct.h"
//#include "edef.h"
//#include "efunc.h
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

#include "mrb_mg.h"
#include "mrb_mode.h"
#include "mrb_extend.h"

mrb_state *mrb;

mrb_value
mrb_s_debug_log(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     char *cstr;
     mrb_get_args(mrb, "S", &value);
     cstr = strndup(RSTRING_PTR(value), RSTRING_LEN(value));
     fprintf(stderr, "[debug]%s\n", cstr);
     return self;
}

mrb_value
mrb_s_dired_backup_unflag(mrb_state *mrb, mrb_value self)
{
     PF func;
     mrb_value value;
     mrb_get_args(mrb, "o", &value);
     func = name_function("dired-backup-unflag");
     (*func)(0, mrb_type(value));
     return mrb_nil_value();
}


int
mrb_load(char *fname)
{
     FILE *f;
     if ((fname = adjustname(fname, TRUE)) == NULL) {
	  /* just to be careful */
	  return (FALSE);
     }
     f = fopen(fname, "r");
     if (f != NULL) {
	  mrb_load_file(mrb, f);
	  fclose(f);
	  return TRUE;
     }
     return FALSE;
}

/* match.c */
mrb_value
mrb_s_showmatch(mrb_state *mrb, mrb_value self)
{
     mrb_value f, n;
     int ret;
     mrb_get_args(mrb, "oo", &f, &n);
     ret = showmatch(mrb_fixnum(f), mrb_fixnum(n));
     return mrb_fixnum_value(ret);
}

/* random.c */
mrb_value
mrb_s_indent(mrb_state *mrb, mrb_value self)
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
    struct RClass *kernel;

    mrb = mrb_open();

    kernel = mrb_class_get(mrb, "Kernel");

    mrb_define_module_function(mrb, kernel, "dired_backup_unflag=", 
			       mrb_s_dired_backup_unflag, ARGS_REQ(1));

    mrb_define_module_function(mrb, kernel, "debug_log", 
			       mrb_s_debug_log, ARGS_REQ(1));

    /* match.c */
    mrb_define_module_function(mrb, kernel, "showmatch",
			       mrb_s_showmatch, ARGS_REQ(2));

    /* random.c */
    mrb_define_module_function(mrb, kernel, "indent",
			       mrb_s_indent, ARGS_REQ(1));

    /* const */
    mrb_define_const(mrb, kernel, "FFRAND", mrb_fixnum_value(FFRAND));

    mrb_mode_init(mrb);
    mrb_extend_init(mrb);
}

//#endif
