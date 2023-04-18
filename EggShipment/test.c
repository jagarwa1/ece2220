
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){


    char date[10] = "10-08-2002";
    int i = 0;
    while (i != strlen(date)){
        if (date[i++] == '-')
            printf("%d\n", atoi(date));
        i++;
    }
}