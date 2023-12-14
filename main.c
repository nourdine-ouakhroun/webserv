#include<stdio.h>
#include<string.h>

// void* memchr(void *val, int c, size_t lenght)
//{
	// char*;

	//i = 0;
	//str[i]
	//	    0x01	  0x02   		0x03						                0x20
	// RAM { 01000100 | 01100101 |  01101100   | | | ||||||| 000000000 ||||  01101100 |||||||||||||||||}
//}

// char* strchr(char *str, int c)
//{
	//i = 0;
	//str[i]
	//	    0x01	  0x02   		0x03						                0x20
	// RAM { 01000100 | 01100101	|  01101100   | | | ||||||| 000000000 ||||  01101100 |||||||||||||||||}
//}
// char * str = hello world; // [h][e][l][l][o][ ][w][o][r][l][d][\0][?]
// int c = l;
// char*valeu = strchr(str, c);
// printf("%c\n", *str)
// 
int main()
{
	size_t k;
	char* i;
}