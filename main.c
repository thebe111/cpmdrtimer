#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

void help(void);
void timer(int timer_val, bool rest);
void error(int line, char *msg);
void notify(char *msg);

int work_timer, rest_timer;

int
main(int argc, char **argv) {
   int opt;

   if (argc == 1) timer(DEFAULT_WORK_VAL, true);

   while ((opt = getopt(argc, argv, "w:r:h")) != -1) {
      switch (opt) {
         case 'w': 
            work_timer = atoi(optarg);
            break;
         case 'r':
            rest_timer = atoi(optarg);
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

   timer(work_timer, true);

   return EXIT_SUCCESS;
}

void
help(void) {
   printf(HELP);
}

void 
timer(int timer_val, bool rest) {
   time_t start, cur, end;
   char key;

   if ((start = time(NULL)) < 0) error(68, "error to start timer");

   end = start + 60 * timer_val;

   if (!rest) notify("\nREST TIME\n");
   else notify("\nWORK TIME\n");

   while(difftime(end, start) > 0) {
      if ((cur = time(NULL)) < 0) error(73, "error to start timer");

      /* TODO: read a signal or a key without block the loop */
      scanf("%c", &key);
      if (key == 's') printf(
            "%02d:%02d\n", 
            (int) difftime(end, cur) / 60,
            (int) difftime(end, cur) % 60
            );
   };

   if (rest) {
      rest_timer ? 
      timer(rest_timer, false) : 
      timer(DEFAULT_REST_VAL, false);
   } else {
      work_timer ? 
      timer(work_timer, true) :
      timer(DEFAULT_WORK_VAL, true);
   }
}

void
error(int line, char *msg) {
   fprintf(stderr, "cpmrtimer:%d: %s [%s]", line, msg, strerror(errno));

   exit(EXIT_FAILURE);
}

void
notify(char *msg) {
   NotifyNotification *ntf;
   notify_init("cpmdrtimer");
   ntf = notify_notification_new("cpmdrtimer", msg, NULL);

   notify_notification_show(ntf, NULL);
}
