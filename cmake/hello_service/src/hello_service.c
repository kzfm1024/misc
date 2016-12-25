#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <mymath.h>

void mylog(int i)
{
  syslog(LOG_INFO, "Hello, systemd %d sqrt %f", i, mysqrt(i));
#ifdef MYLOG_DEBUG
  printf("Hello, systemd %d sqrt %f\n", i, mysqrt(i));
#endif /* MYLOG_DEBUG */
}

int main(int argc, char** argv)
{
  openlog(argv[0], LOG_PID, LOG_DAEMON);

  int i = 0;
  while (1) {
    mylog(i++);
    sleep(10);
  }

  closelog();

  return 0;
}
