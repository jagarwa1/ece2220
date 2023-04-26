/* This Program Demonstrates Writing to Another 
   Terminal by Opening Device File 
   
   Make sure you have at least two terminals open 
   when executing this program.
   
   Use ps -U userid to get list of open terminal numbers
*/
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{ FILE *fpt;
  int Terminal;
  char strDev[100];
  
  if (argc != 2)
  { printf("Usage: devterm n, where n is the terminal number.\n");
    exit(1);
  }
  else Terminal = atoi(argv[1]);
  
  sprintf(strDev, "/dev/pts/%d", Terminal);

  if ((fpt = fopen(strDev,"w")) == NULL)
  { printf("Unable to open %s\n", strDev);
    exit(2);
  }
  fprintf(fpt,"\nHello!  Scientia Est Potentia!!!\n");
  
  fclose(fpt);
  exit(0);
}
