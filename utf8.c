#include <string.h>
#include "def.h"
#include "utf8.h"

int utf8_bytes(char *line, int index)
{ 
    unsigned char c = line[index];

     if (0x01 <= c && c <= 0x7F) {
	  return 1;
     }
     if (0xC0 <= c && c <= 0xDF) {
	  return 2;
     }
     if (0xE0 <= c && c <= 0xEF) {
	  return 3;
     }
     return 1;
}


int utf8_nlength(char *line, int size)
{
     int num_of_char = 0;
     int i = 0;
     while (i < size) {
	  i += utf8_bytes(line, i);
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
     int s1_bytes = utf8_bytes(str1, index1);
     int s2_bytes = utf8_bytes(str2, index2);
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
     
