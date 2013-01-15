#ifndef MRB_MODE_H
#define MRB_MODE_H

extern void mrb_mode_init(void);
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

static PF mrb_mode_funcs[] = {
     mrb_mode1,
     mrb_mode2,
     mrb_mode3,
     mrb_mode4,
     mrb_mode5,
     mrb_mode6,
     mrb_mode7,
     mrb_mode8,
     mrb_mode9,
     mrb_mode10
};


#endif /* MRB_MODE_H */
