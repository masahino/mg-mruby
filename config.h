#define FKEYS
#define XKEYS
#ifndef LOGIN_NAME_MAX
#define LOGIN_NAME_MAX _POSIX_LOGIN_NAME_MAX
#endif
#ifndef MAXLOGNAME
#define MAXLOGNAME LOGIN_NAME_MAX
#endif
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
#define HAVE_ARC4RANDOM
#define HAVE_NOSTRTONUM
#define HAVE_ST_MTIMESPEC
#define UTF8
#define ICONV
#define MRUBY
