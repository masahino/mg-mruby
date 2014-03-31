#include <string.h>
#include "def.h"
#include "key.h"
#include "kbd.h"
#include "funmap.h"
#include "utf8.h"

extern int changemode(int, int, char *);
static int utf8_insert(int f, int n);

static PF utf8_pf[] = {
	utf8_insert, /* 0xE0 */
	utf8_insert, /* 0xE1 */
	utf8_insert, /* 0xE2 */
	utf8_insert, /* 0xE3 */
	utf8_insert, /* 0xE4 */
	utf8_insert, /* 0xE5 */
	utf8_insert, /* 0xE6 */
	utf8_insert, /* 0xE7 */
	utf8_insert, /* 0xE8 */
	utf8_insert, /* 0xE9 */
	utf8_insert, /* 0xEA */
	utf8_insert, /* 0xEB */
	utf8_insert, /* 0xEC */
	utf8_insert, /* 0xED */
	utf8_insert, /* 0xEE */
	utf8_insert, /* 0xEF */
};

static struct KEYMAPE (1 + IMAPEXT) utf8map = {
	1,
	1 + IMAPEXT,
	rescan,
	{
		{ 0xe0, 0xef, utf8_pf, NULL }
	}
};

/* bytes */
int utf8_bytes(char *line, int index, int counts)
{ 
     unsigned char *p;
     int bytes = 0;
     int total_bytes = 0;
     int i;

     if (line == NULL) {
	  return 0;
     }
     p = (unsigned char*)line;

     for (i = 0; i < counts; i++) {
	  if (0x01 <= *p && *p <= 0x7F) {
	       bytes = 1;
	  } else if (0xC0 <= *p && *p <= 0xDF) {
	       bytes = 2;
	  } else if (0xE0 <= *p && *p <= 0xEF) {
	       bytes = 3;
//	  } else if (0x80 <= c && c <= 0x8F && index > 0) {
//	       bytes += utf8_bytes(line, index - 1, 1);
//	       c = line[index + bytes];
	  } else {
	       bytes = 1;
	  }
	  total_bytes += bytes;
	  p += bytes;
     }
     return total_bytes;
}

/* num of chars */
int utf8_nlength(char *line, int size)
{
     int num_of_char = 0;
     int i = 0;
     
     if (line == NULL) {
	  return 0;
     }
     while (i < size) {
	  i += utf8_bytes(line, i, 1);
	  num_of_char++;
     }
     return num_of_char;
}

int utf8_length(char *line)
{
     return utf8_nlength(line, strlen(line));
}

int utf8_cmp(char *str1, int index1, char *str2, int index2)
{
     int s1_bytes = utf8_bytes(str1, index1, 1);
     int s2_bytes = utf8_bytes(str2, index2, 1);
     int i;

     if (s1_bytes != s2_bytes) {
	  return FALSE;
     }
     for (i = 0; i < s1_bytes; i++) {
	  if (str1[index1++] != str2[index2++]) {
	       return FALSE;
	  }
     }
     return TRUE;
}

int
utf8mode(int f, int n)
{
	do_meta(FFARG, 0);
	return(changemode(f, n, "utf8"));
}
     
void
utf8_init(void)
{
	funmap_add(utf8mode, "utf8");
	maps_add((KEYMAP *)&utf8map, "utf8");
}


static int 
utf8_insert(int f, int n)
{
	char utf8_str[3];
	utf8_str[0] = key.k_chars[key.k_count - 1];
	utf8_str[1] = ttgetc();
	utf8_str[2] = ttgetc();

	linsert_str(utf8_str, 3);	
	return TRUE;
}
