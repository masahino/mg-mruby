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

#include "mrb_mg.h"
#include "mrb_mode.h"
#include "mrb_funmap.h"

struct mrb_funmap {
     int mode_num;
};

void mrb_funmap_free(mrb_state *mrb, void *ptr)
{
     mrb_free(mrb, ptr);
}

struct mrb_data_type mrb_funmap_type = {
     "Funmap",
     mrb_funmap_free
};

mrb_value mrb_funmap_initialize(mrb_state *mrb, mrb_value self)
{
     struct mrb_funmap *map;
     
     map = mrb_malloc(mrb, sizeof(struct mrb_funmap));
     map->mode_num = 0;
     mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
		     Data_Wrap_Struct(mrb, (struct RClass*) &self,
				      &mrb_funmap_type, (void*) map)));
     return self;
}

mrb_value mrb_funmap_add(mrb_state *mrb, mrb_value self)
{
     mrb_value fun_name, method_name, result;
     mrb_value value;
     struct mrb_funmap *funmap;

     mrb_get_args(mrb, "SS", &method_name, &fun_name);

     value = mrb_iv_get(mrb, self, mrb_intern(mrb, "data"));
     Data_Get_Struct(mrb, value, &mrb_funmap_type, funmap);
     funmap_add(mrb_mode_funcs[funmap->mode_num], RSTRING_PTR(fun_name));
     funmap->mode_num++;
     return result;
}

void mrb_funmap_init()
{
     struct RClass *funmap;
     
     funmap = mrb_define_class(mrb, "Funmap", mrb->object_class);
     mrb_define_method(mrb, funmap, "initialize", 
		       mrb_funmap_initialize, ARGS_ANY());
     mrb_define_method(mrb, funmap, "add", 
			     mrb_funmap_add, ARGS_REQ(2));

}
