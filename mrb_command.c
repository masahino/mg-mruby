#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/hash.h>
#include <mruby/string.h>

#include "def.h"
#include "funmap.h"

#include "mrb_command.h"

extern mrb_state *mrb;
/*
	dummy function
*/
int
mrb_command_function(int f, int n)
{
    return TRUE;
}

/*
*/
int
mrb_command_callback(int f, int n, mrb_value callback)
{
    mrb_value argv[2];
    argv[0] = mrb_fixnum_value(f);
    argv[1] = mrb_fixnum_value(n);
    mrb_yield_argv(mrb, callback, 2, argv);

    return TRUE;
}

mrb_value
mrb_add_command(mrb_state *mrb, mrb_value self)
{
    mrb_value command_name, block, command_list;
    mrb_get_args(mrb, "&S", &block, &command_name);

    command_list = mrb_cv_get(mrb, self, mrb_intern(mrb, "@@mode_list"));
    
    funmap_add_mrb(block, RSTRING_PTR(command_name));
    mrb_hash_set(mrb, command_list, command_name, block);

    return mrb_nil_value();
}


void mrb_command_init(mrb_state *mrb)
{
     struct RClass *mg;
     mrb_value command_list;

     mg = mrb_class_get(mrb, "MG");
     
     mrb_define_module_function(mrb, mg, "add_command",
				mrb_add_command, ARGS_REQ(2));
     command_list = mrb_hash_new(mrb);
     mrb_mod_cv_set(mrb, mg, mrb_intern(mrb, "@@command_list"), command_list);
     
}
