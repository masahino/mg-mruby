#include <string.h>
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
