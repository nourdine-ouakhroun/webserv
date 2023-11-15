#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// little endian if true
bool endian()
{
	int n = 1;
	return (*(char *)&n);
}

void ibm () {
    FILE* fp;

    /* Our example data structure */
    struct {
        char one[4];
        int  two;
        char three[4];
    } data;

    /* Fill our structure with data */
    strcpy (data.one, "foo");
    data.two = 0x01234567;
    strcpy (data.three, "bar");

    /* Write it to a file */
    fp = fopen ("output", "wb");
    if (fp) {
        fwrite (&data, sizeof (data), 1, fp);
        fclose (fp);
    }
    
}

int main() {
    unsigned int num = 1145258561;      // 01000100 01000011 01000010 01000001
                                        //     D        C        B       A    
    char c = (char)num;
    printf("address %p, vlaue %c\n", &num, num);
    printf("address %p, vlaue %c\n", &num + 1, num + 1);
    printf("address %p, vlaue %c\n", &num + 2, num + 2);
    printf("address %p, vlaue %c\n", &num + 3, num + 3);
    printf("\n");
    printf("address %p, vlaue %c\n", &c, c);
    printf("address %p, vlaue %c\n", &c + 1, c + 1);
    printf("address %p, vlaue %c\n", &c + 2, c + 2);
    printf("address %p, vlaue %c\n", &c + 3, c + 3);
	if (endian())
		printf("little-endian\n");
	else
		printf("big-endian\n");
    ibm();
    return 0;
}
