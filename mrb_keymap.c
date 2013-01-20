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
#include "mrb_keymap.h"

static int mrb_keymap_callback(int f, int n);

/* mappings for all "printable" characters ('-' -> '~') */
static PF mrb_charmap[] = {
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback, 
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback, mrb_keymap_callback,
     mrb_keymap_callback, mrb_keymap_callback
};

static struct KEYMAPE (1) mrb_map = {
     1,
     1,
     rescan,
     {
	  {' ', '~', mrb_charmap, NULL}
     }
};

struct mrb_keymap {
     mrb_value callback_data; /* Hash */
     KEYMAP keymap;
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

static int mrb_keymap_callback(int f, int n)
{
     struct maps_s *cur_map;
     struct mrb_mode *cur_mode;
     mrb_value key_hash, keys;
     int i;

     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     cur_mode = mrb_modes;
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     cur_map = curbp->b_modes[curbp->b_nmodes];
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     while (cur_mode != NULL) {
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
	  if (strcmp(cur_mode->mode_name, cur_map->p_name) == 0) {
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
	       break;
	  }
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
	  cur_mode = cur_mode->next;
     }
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     if (cur_mode == NULL) {
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
	  return rescan(f, n);
     }
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);

     fprintf(stderr, "%c \n", key.k_chars[key.k_count -1]);
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     key_hash = cur_mode->callback_h;
     fprintf(stderr, "key_hash = %s\n", RSTRING_PTR(mrb_any_to_s(mrb, key_hash)));
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     keys =
	  mrb_hash_keys(cur_mode->mrb, 
			key_hash);
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     fprintf(stderr, "%c \n", key.k_chars[key.k_count -1]);
     
     return rescan(f, n);
}


mrb_value mrb_keymap_define_key(mrb_state *mrb, mrb_value self)
{
     mrb_value key_str, method_name, map_h;
     mrb_value value;
     mrb_sym s;
     struct mrb_keymap *map;

//     mrb_get_args(mrb, "H", &key_str, &method_name);
     mrb_get_args(mrb, "H", &map_h);
     value = mrb_iv_get(mrb, self, mrb_intern(mrb, "data"));
     Data_Get_Struct(mrb, value, &mrb_keymap_type, map);
     map->callback_data = map_h;
     fprintf(stderr, "map_h %s\n", RSTRING_PTR(mrb_any_to_s(mrb, map_h)));
     fprintf(stderr, "map_h %s\n", RSTRING_PTR(mrb_any_to_s(mrb, map->callback_data
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
//     mrb_keymap_set(map);

//     fprintf(stderr, "key_str = %s\n", RSTRING_PTR(key_str));
//     fprintf(stderr, "%s\n", RSTRING_PTR(mrb_any_to_s(mrb, method_name)));
//     mrb_funcall(mrb, self, "__send__", 3, method_name, mrb_fixnum_value(0), mrb_fixnum_value(0));

     maps_add((KEYMAP *)&mrb_map, "ruby");
     fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
     return self;
}

mrb_value mrb_keymap_initialize(mrb_state *mrb, mrb_value self)
{
     struct mrb_keymap *map;
     map = malloc(sizeof(struct mrb_keymap));
     mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
		     Data_Wrap_Struct(mrb, (struct RClass*) &self,
				      &mrb_keymap_type, (void*)map)));
     return self;
}


/*
mrb_value mrb_keymap_initialize2(mrb_state *mrb, mrb_value self)
{
     mrb_value map_array, mode_name;
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
//     mape.k_funcp = mrb_mode1;
     mape.k_prefmap = NULL;
     mrb_mape = malloc(sizeof(struct mrb_map_element));
     memcpy(&(mrb_mape->mape), &mape, sizeof(struct map_element));
     mrb_mape->method_name = k_funcname;
     mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
		     Data_Wrap_Struct(mrb, (struct RClass*) &self,
				      &mrb_map_element_type, (void*) mrb_mape)));
     return self;
}
*/
void mrb_keymap_init(mrb_state *mrb)
{
     struct RClass *keymap, *mape;
     
     keymap = mrb_define_class(mrb, "Keymap", mrb->object_class);
//     MRB_SET_INSTANCE_TT(keymap, MRB_TT_DATA);
     mrb_define_method(mrb, keymap, "initialize", 
		       mrb_keymap_initialize, ARGS_NONE());
     mrb_define_method(mrb, keymap, "define_key", mrb_keymap_define_key, 
		       ARGS_ANY());
//     mrb_define_method(mrb, keymap, "add", 
//			     mrb_keymap_add, ARGS_REQ(2));

/*
     mape = mrb_define_class(mrb, "MapElement", mrb->object_class);
     MRB_SET_INSTANCE_TT(mape, MRB_TT_DATA);
     mrb_define_method(mrb, mape, "initialize",
		       mrb_mape_initialize, ARGS_REQ(4));
*/
}

