/*	$OpenBSD: funmap.h,v 1.7 2008/06/10 00:19:31 kjell Exp $	*/

/* This file is in the public domain */

void		 funmap_init(void);
PF		 name_function(const char *);
const char	*function_name(PF);
struct list	*complete_function_list(const char *);
int		 funmap_add(PF, const char *);
#ifdef MRUBY
int              funmap_add_mrb(mrb_value, const char *);
mrb_value        name_function_mrb_block(const char *);
#endif /* MRUBY */
