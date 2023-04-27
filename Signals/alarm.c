/* This program demonstrates the use of the alarm() function
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef void (*fptr)();

void CtrlCHandler(int signum)
{ static int i;

  printf("\tCTRL-C Sent.\n");
  if (++i > 5)
  { exit(0);
  }
}
/*************************************************/
void AlarmHandler(int signum)
{ static int i;
  int n;

  if (++i > 20)
  { exit(0);
  }
  printf("\tAlarm Signalled!!!.\n");
  alarm(n = (rand() % 5 + 1));  /* Reset Alarm to Random seconds */
  printf("\tSet Alarm for %d seconds.\n", n);
}
/*************************************************/
void main(void)
{
  signal(SIGINT, (fptr)CtrlCHandler);	/* Prepare for CTRL-Cs */
  signal(SIGALRM, (fptr)AlarmHandler);  /* Prepare for Alarms  */
  alarm(1);		/* Set alarm to go off in one second */
  printf("Set Alarm for 1 second.\n");

  while (1)
  { printf("Waiting for Alarm or CTRL-C...\n");
    sleep(1);
  }
}

