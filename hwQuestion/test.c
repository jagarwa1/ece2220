
#include <stdio.h>
#include <unistd.h>

void Do1(void)
{ printf("1"); fflush(stdout); sleep(1);
}

void Do2(void)
{ printf("2"); fflush(stdout); sleep(3);
}

void Do3(void)
{ printf("3"); fflush(stdout); sleep(2);
}

int main(void)
{ int wait_ret;

  if (fork() != 0)
  { if (fork() != 0)
    { if (fork() != 0)
      { wait_ret = wait(NULL);
        printf("4"); fflush(stdout);

        wait_ret = wait(NULL);
        printf("5"); fflush(stdout);

        wait_ret = wait(NULL);
        printf("6"); fflush(stdout);
      }
      else Do1();
    }
    else Do2();
  }
  else Do3();
}