/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "def.h"
#include "funmap.h"
#include "kbd.h"
#include "key.h"

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/class.h"
#include "mruby/compile.h"
#include "mruby/hash.h"

#include "mrb_mg.h"
#include "mrb_mode.h"

extern int changemode(int, int, char *);

mrb_value
mrb_change_mode(mrb_state *mrb, mrb_value self)
{
     mrb_value mode_name;
     
     mrb_get_args(mrb, "S", &mode_name);
     if (name_mode(RSTRING_PTR(mode_name)) != NULL) {
	  changemode(0, 1, RSTRING_PTR(mode_name));
     }
     return self;
}

void mrb_mode_init(mrb_state *mrb)
{
     struct RClass *mg;
     mrb_value mode_list;

     maps = NULL;
     mg = mrb_module_get(mrb, "MG");
     
     mrb_define_module_function(mrb, mg, "change_mode",
				mrb_change_mode, ARGS_REQ(1));
     mode_list = mrb_hash_new(mrb);
     mrb_mod_cv_set(mrb, mg, mrb_intern_cstr(mrb, "@@mode_list"), mode_list);
     
}
