#include <stdio.h>
char *
fparseln(FILE *stream, size_t *len, size_t *lineno, const char delim[3], int flags)
{
     size_t s, i = 0;
     char *line = NULL, *p;
     char *buf = NULL;
     line = fgetln(stream, &s);
     buf = malloc(s);
     memset(buf, '\0', s);
     if (line == NULL)
	  return NULL;
     if (s != NULL) {
	  for (p = line; p < line + s; p++) {
	       if (*p != delim[0] && *p != '\n') {
		    buf[i++] = *p;
	       }
	  }	    
     }
     return buf;
}
