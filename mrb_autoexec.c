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
#include "mruby/string.h"

#include "mrb_autoexec.h"

#define MRB_AUTOEXEC_MAX 10

struct mrb_autoexec {
     mrb_state *mrb;
     const char *pattern;
     mrb_value callback;
};

PF mrb_autoexec_funcs[] = {
     mrb_autoexec0,
     mrb_autoexec1,
     mrb_autoexec2,
     mrb_autoexec3,
     mrb_autoexec4,
     mrb_autoexec5,
     mrb_autoexec6,
     mrb_autoexec7,
     mrb_autoexec8,
     mrb_autoexec9,
};

struct mrb_autoexec mrb_autos[MRB_AUTOEXEC_MAX];;
int mrb_autoexec_num = 0;

static int
mrb_autoexec(int f, int n, int i)
{
     mrb_state *mrb;

     mrb = mrb_autos[i].mrb;
     fprintf(stderr, "mrb_autoexec1\n");
     mrb_yield(mrb, 
	       mrb_autos[i].callback,
	       mrb_str_new_cstr(mrb, mrb_autos[i].pattern));
     return TRUE;
}

int
mrb_autoexec0(int f, int n)
{
     return mrb_autoexec(f, n, 0);
}

int
mrb_autoexec1(int f, int n)
{
     return mrb_autoexec(f, n, 1);
}

int
mrb_autoexec2(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec3(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec4(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec5(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec6(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec7(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec8(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

int
mrb_autoexec9(int f, int n)
{
     return mrb_autoexec(f, n, 1);
     return 0;
}

/*
     auto‐execute
            Register an auto‐execute hook; that is, specify a filename pattern
            (conforming to the shell’s filename globbing rules) and an associ‐
            ated function to execute when a file matching the specified pat‐
            tern is read into a buffer.
*/
mrb_value
mrb_auto_execute(mrb_state *mrb, mrb_value self)
{
     mrb_value pat, block;
     int s;
     char autoexec_name_str[32];

     if (mrb_autoexec_num >= MRB_AUTOEXEC_MAX) {
	  return mrb_nil_value();
     }

     mrb_get_args(mrb, "&S", &block, &pat);
     fprintf(stderr, "pat = %s\n", RSTRING_PTR(mrb_any_to_s(mrb, pat)));
     fprintf(stderr, "block = %s\n", RSTRING_PTR(mrb_any_to_s(mrb, block)));

     mrb_autos[mrb_autoexec_num].mrb = mrb;
     mrb_autos[mrb_autoexec_num].pattern = RSTRING_PTR(pat);
     mrb_autos[mrb_autoexec_num].callback = block;

     sprintf(autoexec_name_str, "mrb_autoexec%d", mrb_autoexec_num);

     funmap_add(mrb_autoexec_funcs[mrb_autoexec_num],
		autoexec_name_str);

     mrb_autoexec_num++;

     if ((s = add_autoexec(RSTRING_PTR(pat), 
			   autoexec_name_str)) != TRUE)
	 return mrb_fixnum_value(s);
     fprintf(stderr, "mrb_autoexec success\n");
     return mrb_true_value();
}

void
mrb_autoexec_init(mrb_state *mrb)
{
    struct RClass *mg;

    mg = mrb_class_get(mrb, "MG");

    mrb_define_module_function(mrb, mg, "auto_execute",
			       mrb_auto_execute, ARGS_REQ(2));

}
