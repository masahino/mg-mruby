#ifndef MRB_MG_H
#define MRB_MG_H

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/compile.h>

extern mrb_state *mrb;

void mrb_mg_init();
int mrb_load(char *fname);


#endif
