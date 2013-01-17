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

extern int changemode(int, int, char *);
extern const char *function_name(PF);

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

int mrb_mode1(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode1));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode2(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode2));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode3(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode3));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode4(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode4));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode5(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode5));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode6(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode6));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode7(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode7));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode8(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode8));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode9(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode9));
     return changemode(f, n, strtok(mode_name, "-"));
}

int mrb_mode10(int f, int n)
{
     char *mode_name;
     mode_name = strdup(function_name(mrb_mode10));
     return changemode(f, n, strtok(mode_name, "-"));
}

mrb_value mrb_mode_handle_special_char(mrb_state *mrb, mrb_value self)
{
     mrb_value value;
     mrb_get_args(mrb, "i", &value);
     return self;
}

static void
mrb_mode_handle_free(mrb_state *mrb, void *p)
{
     
}

static const struct mrb_data_type mrb_mode_data_type = {
     "mrb_mode", mrb_mode_handle_free,
};

mrb_value mrb_mode_initialize(mrb_state *mrb, mrb_value self)
{
     mrb_value mode_name;
     struct mrb_mode *mode_data;
     char *mode_name_str;
     
     if (mrb_mode_num >= MRB_MODE_MAX) {
	  return mrb_nil_value();
     }

     mode_data = malloc(sizeof(struct mrb_mode));
     
     mrb_get_args(mrb, "S", &mode_name);
     // TODO: duplicate check !!
     mode_data->mrb = mrb;
     mode_data->mode_name = RSTRING_PTR(mode_name);
     mode_data->funmap = malloc(sizeof(struct mrb_funmap));

     mode_name_str = RSTRING_PTR(mrb_str_cat2(mrb, mode_name, "-mode"));
     funmap_add(mrb_mode_funcs[mrb_mode_num], mode_name_str);
     mrb_mode_num++;

     mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
		     Data_Wrap_Struct(mrb, (struct RClass*) &self,
				      &mrb_mode_data_type, (void*)mode_data)));
     return self;
/*
  mrb_uv_data* uvdata = (mrb_uv_data*) malloc(sizeof(mrb_uv_data));
  uvdata->mrb = mrb;
  uvdata->data = malloc(sizeof(uv_timer_t));
  uvdata->proc = mrb_nil_value();
  uv_timer_init(uv_default_loop(), (uv_timer_t*) uvdata->data);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "data"), mrb_obj_value(
    Data_Wrap_Struct(mrb, (struct RClass*) &self,
    &uv_handle_data_type, (void*) uvdata)));
  return self;     
*/

}

void mrb_mode_init(mrb_state *mrb)
{
     struct RClass *mode;
     
     mode = mrb_define_class(mrb, "Mode", mrb->object_class);
     mrb_define_method(mrb, mode, "initialize", mrb_mode_initialize, 
		       ARGS_REQ(1));
     mrb_define_method(mrb, mode, "handle_special_char", 
		       mrb_mode_handle_special_char, ARGS_REQ(1));

}
