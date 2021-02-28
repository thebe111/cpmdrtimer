#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* necessary to use getch(), don't need to press enter */
/* #include <ncurses.h> */

#define HELP "USAGE: cpmdrtimer \
-t <int> \
-r <int> \
\n\n\
OPTIONS: \n \
-t   work time \n \
-r   rest time \n \
-h   this help message \n"

void help();
void timer(int time);
void rest();

int main(int argc, char **argv) {
   int opt;

   /* initscr(); */

   while ((opt = getopt(argc, argv, "t:r:h")) != -1 ) {
      switch (opt) {
         case 't': 
            timer(atoi(optarg));
            break;
         case 'r':
            rest(optarg);
            break;
         case 'h':
            help();
            break;
         default:
            help();
            exit(1);
            break;
      }
   }

   return 0;
}

void help() {
   printf(HELP);
}

void timer(int time) {
   char key;
   unsigned int cur = 0, end = time * 1000 * 60 * 60;
   clock_t start = clock();

   while(cur < end) {
      clock_t diff = clock() - start;
      cur = (diff * 1000 * 60) / CLOCKS_PER_SEC; 

      scanf("%c", &key);
      if (key == 's') printf("%d\n", ((time * cur) / end));
   };

   printf("END");
}

void rest(int rest) {
}
