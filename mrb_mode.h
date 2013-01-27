#ifndef MRB_MODE_H
#define MRB_MODE_H

#define MRB_MODE_MAX 10

struct mrb_mode {
     mrb_state *mrb;
     mrb_value mode_obj;
     const char *mode_name;
     mrb_value callback_h;
     struct mrb_mode *next;
};


extern void mrb_mode_init(mrb_state *mrb);
extern int mrb_mode1(int f, int n);
extern int mrb_mode2(int f, int n);
extern int mrb_mode3(int f, int n);
extern int mrb_mode4(int f, int n);
extern int mrb_mode5(int f, int n);
extern int mrb_mode6(int f, int n);
extern int mrb_mode7(int f, int n);
extern int mrb_mode8(int f, int n);
extern int mrb_mode9(int f, int n);
extern int mrb_mode10(int f, int n);

extern PF mrb_mode_funcs[];
extern struct mrb_mode *mrb_modes;

#endif /* MRB_MODE_H */
