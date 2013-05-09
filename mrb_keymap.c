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
#include "mrb_keymap.h"

mrb_value
mrb_mg_set_key(mrb_state *mrb, mrb_value self, KEYMAP *map)
{
     mrb_value block;
     mrb_value keymap_list;
     mrb_value *argv;
     int argc;
     int ret;
     mrb_get_args(mrb, "&*", &block, &argv, &argc);
     if (mrb_nil_p(block)) {
	  if (argc == 2 && mrb_string_p(argv[0]) && mrb_string_p(argv[1])) {
	       ret = dobindkey(map, RSTRING_PTR(argv[1]), RSTRING_PTR(argv[0]));
	       return mrb_fixnum_value(ret);
	  } 
     } else {
	  if (argc == 1) {
	       keymap_list = mrb_gv_get(mrb, 
					mrb_intern(mrb, "$mg_keymap_list"));
	       if (!mrb_nil_p(keymap_list)) {
		    mrb_hash_set(mrb, keymap_list, argv[0], argv[0]);
		    return mrb_true_value();
	       }
	  }
     } 
     return mrb_nil_value();
}

/*
     global-set-key
            Bind a key in the global (fundamental) key map.
*/
mrb_value
mrb_mg_global_set_key(mrb_state *mrb, mrb_value self)
{
     mrb_value key, func;
     mrb_value obj1, obj2;
     int ret;

     return mrb_mg_set_key(mrb, self, fundamental_map);

//     mrb_get_args(mrb, "SS", &key, &func);
     mrb_get_args(mrb, "oo", &obj1, &obj2);
     if (mrb_symbol_p(obj1)) {
			
     } else {
         ret = dobindkey(fundamental_map, RSTRING_PTR(func), RSTRING_PTR(key));
         return mrb_fixnum_value(ret);
    }
}

/*
     local-set-key
            Bind a key mapping in the local (topmost) mode.
*/
mrb_value
mrb_mg_local_set_key(mrb_state *mrb, mrb_value self)
{
     mrb_value key, func;
     int ret;

     mrb_get_args(mrb, "SS", &key, &func);
     ret = dobindkey(curbp->b_modes[curbp->b_nmodes]->p_map, RSTRING_PTR(func), RSTRING_PTR(key));
     return mrb_fixnum_value(ret);
}


void mrb_keymap_init(mrb_state *mrb)
{
     struct RClass *mode, *mg;
     mrb_value keymap_list;

     maps = NULL;
     mg = mrb_class_get(mrb, "MG");
     
     mrb_define_module_function(mrb, mg, "global_set_key",
				mrb_mg_global_set_key, MRB_ARGS_ANY());

     mrb_define_module_function(mrb, mg, "local_set_key",
			       mrb_mg_local_set_key, MRB_ARGS_REQ(2));
     keymap_list = mrb_hash_new(mrb);
     mrb_gv_set(mrb, mrb_intern(mrb, "$mg_keymap_list"), keymap_list);
     
}
