#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>


void printBits(unsigned int num){
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1<<(size*8-1);
    printf("MAX POW : %u\n",maxPow);
    unsigned int i=0;
    for(;i<size*8;++i){
        printf("%u ",num&maxPow ? 1 : 0);
        num = num<<1;
    }
}

void printbincharpad(char *s)
{
    unsigned int rest = 0;
    for (unsigned int i = 0; s[i]; i++)
    {
        printf("%c", s[i]);
        if (s[i] == '.')
            continue ;
        unsigned int a = s[i] - 48;
        for (int j = 7; j >= 0; --j)
            rest += (a & (1 << j)) ? 1 : 0;
    }
    printf("%d\n", rest);
}

int main(int argc, char **argv) {

    if (argc >= 2) {
        struct sockaddr_in sa;

        inet_pton(AF_INET, argv[1], &sa.sin_addr);
        printf("%c\n", argv[1][0]);
        printbincharpad(argv[1]);
        /* for verifying purposes, I'm trying to print the result
           to make sure I get a binary number (obviously, "%d" should
           not print a binary yet I do get an int result) */
        // printBits(sa.sin_addr.s_addr);
        // printf("\n%u\n", sa.sin_addr.s_addr);

    }
}