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
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/class.h"
#include "mruby/compile.h"

#include "mrb_mg.h"
#include "mrb_mode.h"
#include "mrb_funmap.h"
#include "mrb_keymap.h"

mrb_state *mrb;

extern int indentmode(int, int);
extern int makebkfile(int, int);
extern int setfillcol(int, int);

mrb_value mrb_s_debug_log(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "S", &value);
     fprintf(stderr, "[debug]%s\n", RSTRING_PTR(value));
     return self;
}

mrb_value mrb_s_auto_indent_mode(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "o", &value);
     indentmode(0, mrb_type(value));
     return mrb_nil_value();
}

mrb_value mrb_s_dired_backup_unflag(mrb_state *mrb, mrb_value self)
{
     PF func;
     mrb_value value;
     mrb_get_args(mrb, "o", &value);
     func = name_function("dired-backup-unflag");
     (*func)(0, mrb_type(value));
     return mrb_nil_value();
}

mrb_value mrb_s_make_backup_files(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "o", &value);
     makebkfile(0, mrb_type(value));
     return mrb_nil_value();
}

mrb_value mrb_s_set_fill_column(mrb_state *mrb, mrb_value self)
{
//     mrb_value value;
//     mrb_get_args(mrb, "i", &value);
//     setfillcol(0, mrb_fixnum(value));
     return mrb_nil_value();
}

mrb_value mrb_s_insert(mrb_state *mrb, mrb_value self)
{
     mrb_value str;
     int ret;
     mrb_get_args(mrb, "S", &str);
     ret = linsert_str(RSTRING_PTR(str), 1);
     return mrb_fixnum_value(ret);
}

int mrb_load(char *fname)
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

void mrb_mg_init()
{
    struct RClass *kernel;

    mrb = mrb_open();
//    mg = mrb_define_class(mrb, "MG", mrb->object_class);
    kernel = mrb_class_get(mrb, "Kernel");

//    mrb_define_method(mrb, mg, "initialize", mrb_mg_initialize, 
//		      ARGS_NONE());

    mrb_define_module_function(mrb, kernel, "auto_indent_mode=", 
			    mrb_s_auto_indent_mode, ARGS_REQ(1));
    mrb_define_module_function(mrb, kernel, "dired_backup_unflag=", 
			    mrb_s_dired_backup_unflag, ARGS_REQ(1));
    mrb_define_module_function(mrb, kernel, "make_backup_files=", 
			    mrb_s_make_backup_files, ARGS_REQ(1));
    mrb_define_module_function(mrb, kernel, "set_fill_column", 
			    mrb_s_set_fill_column, ARGS_REQ(1));

    mrb_define_module_function(mrb, kernel, "insert",
			    mrb_s_insert, ARGS_REQ(1));

    mrb_define_module_function(mrb, kernel, "debug_log", 
			    mrb_s_debug_log, ARGS_REQ(1));
    
    mrb_funmap_init(mrb);
    mrb_keymap_init(mrb);
    mrb_mode_init(mrb);
//    mrb_gc_arena_restore(mrb, 0);
}

//#endif
