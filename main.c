#include <stdio.h>

#include "my_atoi.c"

static char HELP_STR[] = "Usage: `rcv <number>`\n";

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "%s", HELP_STR);
    return 1;
  }

  fprintf(stdout, "%d\n", my_atoi(argv[1]));
  return 0;
}
