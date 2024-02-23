#include <unistd.h>

int main()
{
    int fd[0];
    pipe(fd);
    write(fd[1], "Hello World", 12);
    char Buffer[12];
    read (fd[0], Buffer, 12);
    printf("%s\n", Buffer);
    return (0);
}