#define FKEYS
#define REGEX
#define XKEYS
#ifndef LOGIN_NAME_MAX
#define MAXLOGNAME _POSIX_LOGIN_NAME_MAX
#endif
#ifndef MAXLOGNAME
#define MAXLOGNAME LOGIN_NAME_MAX
#endif
#define HAVE_ARC4RANDOM
#define HAVE_NOSTRTONUM
#define UTF8
#define MRUBY
