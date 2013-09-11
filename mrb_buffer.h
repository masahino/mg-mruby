#ifndef MRB_BUFFER_H
#define MRB_BUFFER_H


extern void mrb_buffer_init(mrb_state *mrb);
extern mrb_value mrb_buffer_obj_value(mrb_state *mrb, struct RClass *class, struct buffer *bp);

#endif /* MRB_BUFFER_H */
