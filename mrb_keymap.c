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
#include "mrb_keymap.h"

#define MRB_MAPE_MAX 10

struct mrb_keymap {
        short   map_num;                        /* elements used */     \
        short   map_max;                        /* elements allocated */\
        PF      map_default;                    /* default function */  \
        struct map_element map_element[MRB_MAPE_MAX];    /* really [e_max] */    \
};

struct mrb_map_element {
     struct map_element mape;
     char *method_name;
};

void mrb_keymap_free(mrb_state *mrb, void *ptr)
{
     mrb_free(mrb, ptr);
}

struct mrb_data_type mrb_keymap_type = {
     "Keymap",
     mrb_keymap_free
};

struct mrb_data_type mrb_map_element_type = {
     "MapElement",
     mrb_keymap_free
};

mrb_value mrb_keymap_initialize(mrb_state *mrb, mrb_value self)
{
     mrb_value map_array, mode_name, result;
     struct mrb_keymap new_map;
     int i, len;
     
     mrb_get_args(mrb, "AS", &map_array, &mode_name);

     len = RARRAY_LEN(map_array);
     new_map.map_num = len;
     new_map.map_max = len;
     new_map.map_default = rescan;
     for (i = 0; i < len; i++) {
	  mrb_value m, mm;
	  struct mrb_map_element *mrb_map;
	  struct map_element *mape;
	  m = mrb_ary_ref(mrb, map_array, i);
	  mm = mrb_iv_get(mrb, m, mrb_intern(mrb, "data"));
	  Data_Get_Struct(mrb, mm, &mrb_map_element_type, mrb_map);
	  mape = &(mrb_map->mape);
	  memcpy(&(new_map.map_element[i]), mape, sizeof(struct map_element));
     }
     maps_add((KEYMAP *)&new_map, RSTRING_PTR(mode_name));
     return mrb_nil_value();
}

mrb_value mrb_mape_initialize(mrb_state *mrb, mrb_value self)
{
     struct map_element mape;
     struct mrb_map_element *mrb_mape;
     char *k_base, *k_num, *k_funcname;
     mrb_value k_prefmap;
     
     mrb_get_args(mrb, "zzzo", &k_base, &k_num, &k_funcname, &k_prefmap);
     mape.k_base = k_base[0];
     mape.k_num = k_num[0];
     mape.k_funcp = mrb_mode1;
     mape.k_prefmap = NULL;
     mrb_mape = malloc(sizeof(struct mrb_map_element));
     memcpy(&(mrb_mape->mape), &mape, sizeof(struct map_element));
     mrb_mape->method_name = k_funcname;
     mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
		     Data_Wrap_Struct(mrb, (struct RClass*) &self,
				      &mrb_map_element_type, (void*) mrb_mape)));
     return self;
}

void mrb_keymap_init()
{
     struct RClass *keymap, *mape;
     
     keymap = mrb_define_class(mrb, "Keymap", mrb->object_class);
     MRB_SET_INSTANCE_TT(keymap, MRB_TT_DATA);
     mrb_define_method(mrb, keymap, "initialize", 
		       mrb_keymap_initialize, ARGS_REQ(2));
//     mrb_define_method(mrb, keymap, "add", 
//			     mrb_keymap_add, ARGS_REQ(2));


     mape = mrb_define_class(mrb, "MapElement", mrb->object_class);
     MRB_SET_INSTANCE_TT(mape, MRB_TT_DATA);
     mrb_define_method(mrb, mape, "initialize",
		       mrb_mape_initialize, ARGS_REQ(4));
}
