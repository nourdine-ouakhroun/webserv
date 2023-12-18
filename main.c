#include<stdio.h>
#include<string.h>

// void* memchr(void *val, int c, size_t lenght)
//{
	// char *;

	//i = 0;
	//str[i]
	//	 0x7ffee6e74787	     				0x7ffee6e74788				 0x7ffee6e74789						0x7ffee6e74790               0x20															0x90
	// [0000000 | 		0000000  | 				000000000   |					 00000000]  

	//	0x7ffee6e74791 		0x7ffee6e74792		0x7ffee6e74793 		0x7ffee6e74794
	// | [000000000| 				000000000  | 	0000000000 |		01100100 ]| 

	// 0x7ffee6e74795			0x7ffee6e74796 		0x7ffee6e74797		0x7ffee6e74798
	//[ 0000000000|				0000000000|				0000000001|			00110100]|

	//	0x7ffee6e74799			0x7ffee6e7479a			0x7ffee6e7479b		0x7ffee6e7479c
	//[ 000000000 |					000000000 |				00000000|			00011110]

//}
// hello world!!    world!
// char* strchr(char *str, int c)
//{
	//i = 0;
	//str[i]
	//	    0x01	  0x02   		0x03			0x04 0x05	0x6             0x20   					0x100
//}
// char * str = hello world; // [h][e][l][l][o][ ][w][o][r][l][d][\0][?]
// int c = l;
// char*valeu = strchr(str, c);
// printf("%c\n", *str)
// 
// Sample program to show that memmove() is better than 
// memcpy() when addresses overlap. 
#include <stdio.h> 
#include <string.h> 
int main() 
{ 
	char str[100] = "Learningisfun"; 
	char str1[100] = "Learningisfun"; 
	char *first, *second; 
	first = str; 
	second = str1; 
	printf("Original string :%s\n ", str); 
	
	// when overlap happens then it just ignore it 
	memcpy(first + 1, first, 10); 
	printf(" memcpy overlap : %s\n ", str); 

	// when overlap it start from first position 
	memmove(second  + 1, second, 10); 
	printf("memmove overlap : %s\n ", str); 

	return 0; 
}

// int main()
// {
	
// 	char *str = "Learningisfun";
// 	char *mem = memmove();
	//00000001 11110100
	// int g[5] = {0 , 100 , 308, 30, 200};
	// char * val = memchr(g, 52, 5 * 4);
	// // printf("%p\n", val);
	// for (size_t i = 0; i < 5 ; i++)
	// {
	// 	printf("%p\n", &(g[i]));
	// }
	// printf("-----------------------\n");
	// printf("%c\n", (*val));
	// printf("%p\n", &(*val));
	// val++;
	// printf("%d\n", (*val));
	// printf("%p\n", &(*val));
	// val--;
	// printf("%d\n", (*val));
	// printf("%p\n", &(*val));
	// val--;
	// printf("%d\n", (*val));
	// printf("%p\n", &(*val));
	// val--;
	// printf("%d\n", (*val));
	// printf("%p\n", &(*val));
	// val--;
	// printf("%d\n", (*val));
	// printf("%p\n", &(*val));
	// // val++;
	// // val++;
	// // printf("%p\n", &(*val));
	// // val++;
	// // printf("%p\n", &(*val));
	// // val++;
	// // printf("%p\n", &(*val));
	// // val++;
	// // printf("%p\n", &(*val));
	// // int l = 'a';
	// //(char)l = 01101001
	// // char srt[] = "hello world";
	// // int c = 'l';
	// // printf("%p\n", &(srt[0]));
	// // printf("%p\n", &(srt[1]));
	// // printf("%s\n", srt);
	// // srt[5] = 0;
	// // printf("%s\n", srt);
	// // printf("%s\n", &srt[6]);
	// // printf("%c", srt[6]);
	// // printf("%c", srt[7]);
	// // printf("%c", srt[8]);
	// // printf("%c", srt[9]);
	// // printf("%c\n", srt[10]);
	// // printf("%p\n", &(srt[2]));
	// // printf("%p\n", &(srt[3]));
	// // printf("%p\n", &c);
// }