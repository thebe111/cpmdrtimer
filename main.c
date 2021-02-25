#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

   while (time * 60 * 60 > 0) {
      time--;

      scanf("%s", key);
      if (key == 't') printf("%d", time);
   }
}

void rest(int rest) {
}
