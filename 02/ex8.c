#include <stdio.h>
#include <string.h>

int main()
{
    char sir[200], subsir[100];
    printf("S: ");
    scanf("%s", sir);

    printf("s: ");
    scanf("%s", subsir);

    char *p = strstr(sir, subsir);
    if (p)
        printf("Subsirul gasit la pozitia %d.\n", p - sir);

    return 0;
}
