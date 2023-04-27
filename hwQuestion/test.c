#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void main(void)
{ int i, j;
  char c[10] = {0,1,2,3,4,5,6,7,8,9};

  for (i=0; i<10; i++)
  { 
    printf("%d", c[++j]);
    if (i == 5)
      *(int*)0 = 0;
  }
}