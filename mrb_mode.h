#ifndef MRB_MODE_H
#define MRB_MODE_H

#define MRB_MODE_MAX 10

struct mrb_funmap {
     mrb_value proc;
     const char *fn_name;
     struct mrb_funmap *fn_next;
};

struct mrb_mode {
     mrb_state *mrb;
     const char *mode_name;
     PF fn_funct;
     struct mrb_funmap *funmap;
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

#endif /* MRB_MODE_H */
