

#include <stdio.h>

void print_red (char *str) {
  printf("\033[1;31m");
  printf("%s \n", str);
  reset();
}

void red(){
    printf("\033[1;31m");
}

void blue() {
  printf("\033[0;34m");
}

void reset () {
  printf("\033[0m");
}
