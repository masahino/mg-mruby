#include <string.h>
#include "def.h"
#include "utf8.h"

/* bytes */
int utf8_bytes(char *line, int index, int counts)
{ 
     unsigned char c;
     int bytes = 0;
     int i;

     if (line == NULL) {
	  return 0;
     }
     c = line[index];

     for (i = 0; i < counts; i++) {
	  if (0x01 <= c && c <= 0x7F) {
	       bytes += 1;
	       c = line[index+bytes];
	  } else if (0xC0 <= c && c <= 0xDF) {
	       bytes += 2;
	       c = line[index+bytes];
	  } else if (0xE0 <= c && c <= 0xEF) {
	       bytes += 3;
	       c = line[index+bytes];
//	  } else if (0x80 <= c && c <= 0x8F && index > 0) {
//	       bytes += utf8_bytes(line, index - 1, 1);
//	       c = line[index + bytes];
	  } else {
	       bytes += 1;
	       c = line[index+bytes];
	  }
     }
     return bytes;
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
     
