#ifndef MRB_HOOK_H
#define MRB_HOOK_H

extern int mrb_hook_callback(char *hook_name, struct buffer *bp);
extern void mrb_hook_init(mrb_state *mrb);

#endif /* MRB_HOOK_H */
