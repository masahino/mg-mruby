#ifndef MRB_COMMAND_H
#define MRB_COMMAND_H

extern int mrb_command_function(int f, int n);
extern int mrb_command_callback(int f, int n, mrb_value callback);
extern void mrb_command_init(mrb_state *mrb);

#endif /* MRB_COMMAND_H */
