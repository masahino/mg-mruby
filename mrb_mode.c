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
#include "mrb_kbd.h"
#include "mrb_mode.h"

extern int changemode(int, int, char *);
extern const char *function_name(PF);

static int mrb_mode_callback(int f, int n);

/* mappings for all "printable" characters ('-' -> '~') */
static PF mrb_charmap[] = {
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback, 
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback, mrb_mode_callback,
     mrb_mode_callback, mrb_mode_callback
};

static PF mrb_charmap_cc[] = {
        mrb_mode_callback,           /* ^C */
        mrb_mode_callback,           /* ^D */
        mrb_mode_callback,           /* ^E */
        mrb_mode_callback,           /* ^F */
        mrb_mode_callback,           /* ^G */
        mrb_mode_callback,           /* ^H */
        mrb_mode_callback,           /* ^I */
        mrb_mode_callback,           /* ^J */
        mrb_mode_callback,           /* ^K */
        mrb_mode_callback,           /* ^L */
        mrb_mode_callback,           /* ^M */
};
static struct KEYMAPE (2) mrb_map = {
     1,
     1,
     rescan,
     {
	  {CCHR('C'), CCHR('M'), mrb_charmap_cc, NULL},
//	  {' ', '~', mrb_charmap, NULL},
     }
};

struct mrb_keymap {
     mrb_value callback_data; /* Hash */
     KEYMAP keymap;
};

PF mrb_mode_funcs[] = {
     mrb_mode1,
     mrb_mode2,
     mrb_mode3,
     mrb_mode4,
     mrb_mode5,
     mrb_mode6,
     mrb_mode7,
     mrb_mode8,
     mrb_mode9,
     mrb_mode10
};

int mrb_mode_num = 0;

struct mrb_mode *mrb_modes;

int
mrb_mode1(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode1));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode2(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode2));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode3(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode3));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode4(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode4));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode5(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode5));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode6(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode6));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode7(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode7));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode8(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode8));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode9(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode9));
     return changemode(f, n, strtok(mode_name, "-"));
}

int
mrb_mode10(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode10));
     return changemode(f, n, strtok(mode_name, "-"));
}

static void
mrb_mode_handle_free(mrb_state *mrb, void *p)
{
     mrb_free(mrb, p);
}

static const struct mrb_data_type mrb_mode_data_type = {
     "mrb_mode", mrb_mode_handle_free,
};

static int 
mrb_mode_callback(int f, int n)
{
     struct maps_s *cur_map;
     struct mrb_mode *cur_mode;
     mrb_value key_hash, keys;
     mrb_value mode_list, mode_tmp;
     int i, ai;
     char *tmp_s;

     mode_list = mrb_gv_get(mrb, mrb_intern(mrb, "$mg_mode_list"));

     cur_map = curbp->b_modes[curbp->b_nmodes];
     ai = mrb_gc_arena_save(mrb);
     mode_tmp = mrb_hash_get(mrb, mode_list,
			     mrb_str_new_cstr(mrb, cur_map->p_name));

     if (mrb_nil_p(mode_tmp)) {
	  return rescan(f, n);
     }
     Data_Get_Struct(mrb, mode_tmp, &mrb_mode_data_type, cur_mode);
     if (mrb_nil_p(cur_mode->callback_h)) {
	  mrb_gc_arena_restore(mrb, ai);
	  return rescan(f, n);
     }
     key_hash = cur_mode->callback_h;
     keys = mrb_hash_keys(cur_mode->mrb, key_hash);
     for (i = 0; i < RARRAY_LEN(keys); i++) {
	  tmp_s = mrb_string_value_ptr(mrb, mrb_ary_entry(keys, i));
	  if (*tmp_s == key.k_chars[key.k_count -1]) {
	       mrb->exc = 0;
	       mrb_funcall(mrb, mrb_top_self(mrb),
			   "__send__", 4,
			   mrb_hash_get(mrb, key_hash, mrb_ary_entry(keys, i)), 
			   mrb_fixnum_value(f),
			   mrb_fixnum_value(n),
			   mrb_ary_entry(keys, i)
		    );
	       if (mrb->exc) {
		    mrb_value obj;
		    obj = mrb_obj_value(mrb->exc);
		    ewprintf("%s", 
			     RSTRING_PTR(mrb_funcall(mrb, obj, "inspect", 0)));
	       }
	       mrb_gc_arena_restore(mrb, ai);
	       return TRUE;

	  }
     }
     mrb_gc_arena_restore(mrb, ai);
     return rescan(f, n);
}

mrb_value
mrb_mode_define_key(mrb_state *mrb, mrb_value self)
{
     mrb_value map_h;
     struct mrb_mode *mode;
     mrb_get_args(mrb, "H", &map_h);
     mode = (struct mrb_mode *)mrb_data_get_ptr(mrb, self, &mrb_mode_data_type);
     mode->callback_h = map_h;
     mrb_iv_set(mrb, self, mrb_intern(mrb, "keymap"), map_h);
     DATA_PTR(self) = mode;
     return self;
}

mrb_value
mrb_mode_initialize(mrb_state *mrb, mrb_value self)
{
     mrb_value mode_name;
     struct mrb_mode *mode_data;

     mode_data = (struct mrb_mode*)DATA_PTR(self);
     if (mode_data) {
	  mrb_mode_handle_free(mrb, mode_data);
     }
     DATA_TYPE(self) = &mrb_mode_data_type;
     DATA_PTR(self) = NULL;

     mrb_get_args(mrb, "S", &mode_name);
     // TODO: duplicate check !!

     mode_data = (struct mrb_mode *)mrb_malloc(mrb, sizeof(*mode_data));
     mode_data->mrb = mrb;
     mode_data->mode_obj = self;
     mode_data->mode_name = strdup(RSTRING_PTR(mode_name));
     mode_data->callback_h = mrb_nil_value();
     mode_data->next = NULL;

     DATA_PTR(self) = mode_data;
     return self;
}

mrb_value
mrb_add_mode(mrb_state *mrb, mrb_value self)
{
     mrb_value mode_obj;
     struct mrb_mode *mode;
     char *mode_name_str;
     mrb_value mode_list;

     if (mrb_mode_num >= MRB_MODE_MAX) {
	  return mrb_nil_value();
     }

     mrb_get_args(mrb, "o", &mode_obj);
     mode = (struct mrb_mode *)mrb_data_get_ptr(mrb, mode_obj,
						&mrb_mode_data_type);
     
     mode->next = mrb_modes;
     mrb_modes = mode;

     maps_add((KEYMAP *)&mrb_map, mode->mode_name);
     mode_name_str = strdup(mode->mode_name);
     mode_name_str = realloc(mode_name_str, strlen(mode_name_str)+6);
     strcat(mode_name_str, "-mode");
     funmap_add(mrb_mode_funcs[mrb_mode_num], mode_name_str);
     mrb_mode_num++;
     
     mode_list = mrb_gv_get(mrb, mrb_intern(mrb, "$mg_mode_list"));
     mrb_hash_set(mrb, mode_list, mrb_str_new_cstr(mrb, mode->mode_name),
		  mode_obj);
     return self;
}


void mrb_mode_init(mrb_state *mrb)
{
     struct RClass *mode, *mg;
     mrb_value mode_list;

     maps = NULL;
     mg = mrb_class_get(mrb, "MG");
     
     mode = mrb_define_class_under(mrb, mg, "Mode", mrb->object_class);
     MRB_SET_INSTANCE_TT(mode, MRB_TT_DATA);
     mrb_define_method(mrb, mode, "initialize", mrb_mode_initialize, 
		       ARGS_REQ(1));
     mrb_define_method(mrb, mode, "define_key", mrb_mode_define_key, 
		       ARGS_REQ(1));

     mrb_define_module_function(mrb, mg, "add_mode",
				mrb_add_mode, ARGS_REQ(1));
     mode_list = mrb_hash_new(mrb);
     mrb_gv_set(mrb, mrb_intern(mrb, "$mg_mode_list"), mode_list);
     
}
