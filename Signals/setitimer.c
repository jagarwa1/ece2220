/* This program demonstrates the use of setitimer() and getitimer().
*/
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

typedef void (*fptr)();

void AlarmHandler(int signum)
{ printf("<*** Timer Expired ***>\n");
}
/*****************************************************/
void main(void)
{ int i;
  struct itimerval TimerValOld, TimerValNew;

  signal(SIGALRM, (fptr)AlarmHandler);  /* Prepare for Alarm  */

  /* Set the Real Timer Values */
  TimerValNew.it_interval.tv_sec = 10;  /* Alarm every 10 seconds   */
  TimerValNew.it_interval.tv_usec = 1;  /*   and one microseconds   */
  TimerValNew.it_value.tv_sec = 5;		/* First Alarm at 5 seconds */
  TimerValNew.it_value.tv_usec = 1;     /*   and one microseconds   */

  printf("Calling setitimer(ITIMER_REAL, &TimerValNew, &TimerValOld)\n");
  setitimer(ITIMER_REAL, &TimerValNew, &TimerValOld);

  printf("Old Interval  (sec): %d\n", (int)TimerValOld.it_interval.tv_sec);
  printf("Old Interval (usec): %d\n\n", (int)TimerValOld.it_interval.tv_usec);
  printf("Old Value     (sec): %d\n", (int)TimerValOld.it_value.tv_sec);
  printf("Old Value    (usec): %d\n\n", (int)TimerValOld.it_value.tv_usec);

  printf("New Interval  (sec): %d\n", (int)TimerValNew.it_interval.tv_sec);
  printf("New Interval (usec): %d\n\n", (int)TimerValNew.it_interval.tv_usec);
  printf("New Value     (sec): %d\n", (int)TimerValNew.it_value.tv_sec);
  printf("New Value    (usec): %d\n\n", (int)TimerValNew.it_value.tv_usec);

  /* Get the Real Timer Values with getitimer */
  for (i=0; i<10; i++)
  { printf ("Calling getitimer(ITIMER_REAL, &TimerValOld)\n");
    getitimer(ITIMER_REAL, &TimerValOld);
    printf("Old Interval  (sec): %d\n", (int)TimerValOld.it_interval.tv_sec);
    printf("Old Interval (usec): %d\n\n", (int)TimerValOld.it_interval.tv_usec);
    printf("Old Value     (sec): %d\n", (int)TimerValOld.it_value.tv_sec);
    printf("Old Value    (usec): %d\n\n", (int)TimerValOld.it_value.tv_usec);
  
    sleep(1);
  }
}

