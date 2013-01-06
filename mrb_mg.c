/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "estruct.h"
//#include "edef.h"
//#include "efunc.h"
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/class.h"
#include "mruby/compile.h"
#include "mrb_mg.h"

mrb_state *mrb = NULL;

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

    mrb_gc_arena_restore(mrb, 0);
}

//#endif
