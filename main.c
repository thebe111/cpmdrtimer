#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
/* #include <getopt.h> */

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

void help();
void timer(int work_val);
void rest(int rest_val);
void fail(int line, char *msg);

int
main(int argc, char **argv) {
   int opt;

   if (argc == 1) printf(HELP);

   while ((opt = getopt(argc, argv, "w:r:h")) != -1 ) {
      switch (opt) {
         case 'w': 
            timer(atoi(optarg));
            break;
         case 'r':
            rest(atoi(optarg));
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

   return EXIT_SUCCESS;
}

void help() {
   printf(HELP);
}

void 
timer(int work_val) {
   char key;
   int end, out;
   time_t start, tmp;
   struct tm *start_localtime, *cur;

   if ((start = time(NULL)) < 0) fail(60, "error to start timer");

   if ((start_localtime = localtime(&start)) == NULL) {
         fail(62, "error to convert start of type time_t to localtime value");
   }

   end = start_localtime->tm_min + work_val;

   while(start_localtime->tm_min < end) {
      if ((tmp = time(NULL)) < 0) fail(69, "error to start timer");

      if ((cur = localtime(&tmp)) == NULL) {
         fail(71, "error to convert tmp of type time_t to localtime value");
      }

      out = end - (start_localtime->tm_min + (cur->tm_min - start_localtime->tm_min)); 

      scanf("%c", &key);
      if (key == 's') printf("%d\n", (
               out != 1 ?
               out :
               60 - localtime(&tmp)->tm_sec
               ));
   };

   printf("END\n");
}

void
rest(int rest_val) {
   printf("rest time: %d\n", rest_val);
}

void
fail(int line, char *msg) {
   fprintf(stderr, "cpmrtimer:%d: %s [%s]", line, msg, strerror(errno));
   exit(EXIT_FAILURE);
}
