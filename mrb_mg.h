#ifndef MRB_MG_H
#define MRB_MG_H

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/compile.h>

extern mrb_state *mrb;

void mrb_mg_init();
int mrb_mg_load(char *fname);
int mrb_mg_evalbuffer(int f, int n);
int mrb_mg_evalexpr(char *line);

#endif /* MRB_MG_H */
