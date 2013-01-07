#ifndef UTF8_H
#define UTF8_H

int utf8_bytes(char *line, int index, int counts);
int utf8_length(char *line);
int utf8_nlength(char *line, int size);
int utf8_cmp(char *str1, int index1, char *str2, int index2);

#endif /* UTF8_H */
