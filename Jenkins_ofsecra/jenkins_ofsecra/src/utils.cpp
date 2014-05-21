#include "utils.h"

void char2byte(char *data, unsigned char *result, int byte_len){
	char *p, b[3], **tmp = 0;
	int i;

	memset(result, 0, byte_len);

	for (p = data, i = 0; p != 0 && i<byte_len; p += 2, i++){
		memset(b, 0, 3);
		strncpy(b, p, 2);
		result[i] = static_cast<unsigned char>(strtol(b, tmp, 16));
	}
	//cout << result; //DEBUG
}

/*void char2byte4(char *data, uint32_t *result, int byte_len){
	char **tmp = 0, *p, b[9];
	int i;
	const int chinbyte4 = 8;
	int res_len = byte_len / 4;

	memset(result, 0, byte_len);

	for (p = data, i = 0; p != 0 && i< res_len; p += chinbyte4, i++){
		memset(b, 0, sizeof(b));
		strncpy(b, p, chinbyte4);
		//swap bytes for Intel
		char c;
		c = b[0]; b[0] = b[6]; b[6] = c;
		c = b[1]; b[1] = b[7]; b[7] = c;
		c = b[2]; b[2] = b[4]; b[4] = c;
		c = b[3]; b[3] = b[5]; b[5] = c;

		result[i] = strtoul(b, tmp, 16);
	}
	//std::cout << result; //DEBUG
}*/

void byte2char(unsigned char *data, char *result, int byte_len, int str_len){
	int i;
	char rgbDigits[] = "0123456789abcdef"; //translate BYTE to char*

	memset(result, 0, str_len);

	for (i = 0; i<byte_len; i++){
		result[2 * i] = rgbDigits[data[i] >> 4];
		result[2 * i + 1] = rgbDigits[data[i] & 0xf];
	}
}

