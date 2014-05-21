#ifndef _UTILS_H
#define _UTILS_H

#include <cstring>
#include <cstdlib>
//#include <cstdint>
#include <iostream>

void char2byte(char *data, unsigned char *result, int byte_len);
void byte2char(unsigned char *data, char *result, int byte_len, int str_len);

//void char2byte4(char *data, uint32_t *result, int byte_len);

#endif
