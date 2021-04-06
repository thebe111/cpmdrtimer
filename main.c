#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
/* #include <getopt.h> */

#ifndef NOTIFY
#include <libnotify/notify.h>
#endif

#define DEFAULT_WORK_VAL 25
#define DEFAULT_REST_VAL 5

#define HELP "USAGE: cpmdrtimer \
-w <int> \
-r <int> \
\n\n\
OPTIONS: \n \
-w, --work   work time \n \
-r, --rest   rest time \n \
-h, --help   this help message \n"

/* TODO: to implement long options */
/* static struct option long_options[] = { */
   /* {"work", no_argument, 0, "w"}, */
   /* {"rest", no_argument, 0, "r"}, */
   /* {"help", no_argument, 0, "h"}, */
   /* {0, 0, 0, 0} */
/* }; */

typedef struct {
   time_t start;
   time_t cur;
   time_t end;
} timerctl_t;

typedef struct {
   int work;
   int rest;
} deftimer_t;

deftimer_t deftimer;
timerctl_t timerctl;
static bool flag = false;

static void help(void);
static void timer(int timer_val);
static void error(int line, char *msg);
static void notify(char *msg);
static void shandler(int signo);
static void cur_time(void);

int
main(int argc, char **argv) {
   int opt;

   if (signal(SIGUSR1, shandler) == SIG_ERR) {
      error(80, "cannot associate SIGUSR1 to handler");
   }

   if (argc == 1) timer(DEFAULT_WORK_VAL);

   while ((opt = getopt(argc, argv, "w:r:h")) != -1) {
      switch (opt) {
         case 'w': 
            deftimer.work = atoi(optarg);
            break;
         case 'r':
            deftimer.rest = atoi(optarg);
            break;
         case 'h':
            help();
            break;
         default:
            help();
            exit(EXIT_FAILURE);
            break;
      }
   }

   timer(deftimer.work);

   return EXIT_SUCCESS;
}

static void
help(void) {
   printf(HELP);
}

static void 
timer(int period) {
   if ((timerctl.start = time(NULL)) < 0) error(68, "error to start timer");

   timerctl.cur = timerctl.start;
   timerctl.end = timerctl.start + 60 * period;

   (flag ^= 1) ? notify("work time") : notify("rest time");

   while(difftime(timerctl.end, timerctl.cur) > 0) {
      if ((timerctl.cur = time(NULL)) < 0) error(73, "error to start timer");
   };

   if (flag) deftimer.rest ? timer(deftimer.rest) : timer(DEFAULT_REST_VAL);
   else deftimer.work ? timer(deftimer.work) : timer(DEFAULT_WORK_VAL);
}

static void
error(int line, char *msg) {
   fprintf(
         stderr, 
         "cpmrtimer:%d: error: %s [%s]\n", 
         line, msg, strerror(errno)
         );

   exit(EXIT_FAILURE);
}

static void
notify(char *msg) {
   NotifyNotification *ntf;
   notify_init("cpmdrtimer");
   ntf = notify_notification_new("cpmdrtimer", msg, NULL);
   notify_notification_show(ntf, NULL);
}

static void
shandler(int signo) {
   if (signo == SIGUSR1) cur_time();
}

static void
cur_time(void) {
   char msg[17];

   sprintf(
         msg,
         "%02d:%02d", 
         (int) difftime(timerctl.end, timerctl.cur) / 60,
         (int) difftime(timerctl.end, timerctl.cur) % 60
         );

   notify(msg);
}
