#ifndef MRB_KBD_H
#define MRB_KBD_H


void mrb_kbd_init(void);
void mrb_keymap_free(mrb_state *mrb, void *ptr);

extern struct mrb_data_type mrb_keymap_type;




#endif /* MRB_KBD_H */
