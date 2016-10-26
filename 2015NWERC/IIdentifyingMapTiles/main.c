#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    char key[30];
    scanf("%s", key);

    int zoom = strlen(key);

    long long x = 0;
    int i;
    for(i = zoom - 1; i>= 0; i--){
        x += ((key[i] == '1') || (key[i] == '3')) * pow(2, (zoom - 1 - i));
    }

    long long y = 0;
    for(i = zoom - 1; i>= 0; i--){
        y += ((key[i] == '2') || (key[i] == '3')) * pow(2, (zoom - 1 - i));
    }

    printf("%d %lld %lld\n", zoom, x, y);

    return 0;
}
