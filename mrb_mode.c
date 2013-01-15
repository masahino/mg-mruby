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
#include "mrb_kbd.h"
#include "mrb_mode.h"


int mrb_mode1(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode2(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode3(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode4(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode5(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode6(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode7(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode8(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode9(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

int mrb_mode10(int f, int n)
{
     return changemode(f, n, strtok(function_name(mrb_mode1), "-"));
}

mrb_value mrb_mode_handle_special_char(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "i", &value);
     return self;
}
mrb_value mrb_mode_initialize(mrb_state *mrb, mrb_value self)
{
     mrb_value mode_name, keymap_obj;
     struct mrb_keymap_s *map;
     mrb_get_args(mrb, "So", &mode_name, &keymap_obj);
//     funmap_add(mrb_mode, 
//		RSTRING_PTR(mrb_str_plus(mrb, mode_name, mrb_str_new2(mrb, "-mode"))));
     map = mrb_get_datatype(mrb, keymap_obj, &mrb_keymap_type);
     maps_add(map, RSTRING_PTR(mode_name));
     return self;
}

void mrb_mode_init()
{
     struct RClass *mode;
     
     mode = mrb_define_class(mrb, "Mode", mrb->object_class);
     mrb_define_method(mrb, mode, "initialize", mrb_mode_initialize, 
		       ARGS_ANY());
     mrb_define_method(mrb, mode, "handle_special_char", 
		       mrb_mode_handle_special_char, ARGS_REQ(1));

}
