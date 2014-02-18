#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/hash.h>
#include <mruby/array.h>
#include <mruby/string.h>

#include "def.h"
#include "funmap.h"

#include "mrb_hook.h"
#include "mrb_buffer.h"

extern mrb_state *mrb;

/*
*/
int
mrb_hook_callback(char *hook_name, struct buffer *bp)
{
    mrb_value hook_list, hook_a, buffer_obj, block;
    mrb_int i;

    hook_list = mrb_mod_cv_get(mrb, mrb_module_get(mrb, "MG"), mrb_intern_cstr(mrb, "@@hook_list"));
    hook_a = mrb_hash_get(mrb, hook_list, mrb_str_new_cstr(mrb, hook_name));

    buffer_obj =  mrb_buffer_obj_value(mrb, mrb_module_get(mrb, "MG"), bp);

    for(i = 0; i < mrb_ary_len(mrb, hook_a); i++) {
        block = mrb_ary_ref(mrb, hook_a, i);
        mrb_yield(mrb, block, buffer_obj);
    }
    return TRUE;
}

mrb_value
mrb_add_hook(mrb_state *mrb, mrb_value self)
{
    mrb_value hook_name, block, hook_list, hook_a;
    mrb_get_args(mrb, "&S", &block, &hook_name);

    hook_list = mrb_cv_get(mrb, self, mrb_intern_cstr(mrb, "@@hook_list"));
    
    hook_a = mrb_hash_get(mrb, hook_list, hook_name);
    if (mrb_nil_p(hook_a)) {
        return mrb_nil_value();
    }
    mrb_ary_push(mrb, hook_a, block);
//    mrb_hash_set(mrb, hook_list, hook_name, block);

    return mrb_nil_value();
}


void mrb_hook_init(mrb_state *mrb)
{
     struct RClass *mg;
     mrb_value hook_list;

     mg = mrb_module_get(mrb, "MG");
     
     mrb_define_module_function(mrb, mg, "add_hook",
				mrb_add_hook, ARGS_REQ(2));
     hook_list = mrb_hash_new(mrb);
     mrb_hash_set(mrb, hook_list, mrb_str_new_cstr(mrb, "write-file-hooks"), mrb_ary_new(mrb));
     mrb_hash_set(mrb, hook_list, mrb_str_new_cstr(mrb, "after-save-hook"), mrb_ary_new(mrb));

     mrb_mod_cv_set(mrb, mg, mrb_intern_cstr(mrb, "@@hook_list"), hook_list);
     
}
