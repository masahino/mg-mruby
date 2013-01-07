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

mrb_state *mrb = NULL;

extern int indentmode(int, int);
extern int makebkfile(int, int);
extern int setfillcol(int, int);

mrb_value mrb_mg_s_auto_indent_mode(mrb_state *mrb, mrb_value self)
{
     indentmode(0, 0);
     return mrb_nil_value();
}

mrb_value mrb_mg_s_dired_backup_unflag(mrb_state *mrb, mrb_value self)
{
     PF func;
     func = name_function("dired-backup-unflag");
     (*func)(0, 0);
     return mrb_nil_value();
}

mrb_value mrb_mg_s_make_backup_files(mrb_state *mrb, mrb_value self)
{
     makebkfile(0, 0);
     return mrb_nil_value();
}

mrb_value mrb_mg_s_set_fill_column(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "i", &value);
     setfillcol(0, mrb_fixnum(value));
     return mrb_nil_value();
}

mrb_value mrb_mg_initialize(mrb_state *mrb, mrb_value self)
{
     return mrb_nil_value();
}					      

void mrb_load(char *fname)
{
     FILE *f;
     f = fopen(fname, "r");
     if (f != NULL) {
	  mrb_load_file(mrb, f);
	  fclose(f);
     }
}

void mrb_mg_init(mrb_state *mrb, mrb_value *self)
{
    struct RClass *mg;

    mg = mrb_define_class(mrb, "MG", mrb->object_class);

    mrb_define_method(mrb, mg, "initialize", mrb_mg_initialize, 
		      ARGS_NONE());

    mrb_define_class_method(mrb, mg, "auto_indent_mode=", 
			    mrb_mg_s_auto_indent_mode, ARGS_NONE());
    mrb_define_class_method(mrb, mg, "dired_backup_unflag=", 
			    mrb_mg_s_dired_backup_unflag, ARGS_NONE());
    mrb_define_class_method(mrb, mg, "make_backup_files=", 
			    mrb_mg_s_make_backup_files, ARGS_NONE());
    mrb_define_class_method(mrb, mg, "set_fill_column=", 
			    mrb_mg_s_set_fill_column, ARGS_REQ(1));

    mrb_gc_arena_restore(mrb, 0);
}

//#endif
