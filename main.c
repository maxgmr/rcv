#include <stdbool.h>
#include <stdio.h>

#include "my_atoi.c"

static char HELP_STR[] = "Usage: `rcv <number>`\n";

int main(int argc, char *argv[]) {
  bool ok = true;
  if (argc != 2) {
    fprintf(stderr, "%s", HELP_STR);
    return 5;
  }

  int result = my_atoi(argv[1], &ok);

  if (!ok) {
    fprintf(stderr, "Failed to parse input as number.\n");
    return 5;
  }

  fprintf(stdout, "%d\n", my_atoi(argv[1], &ok));
  return 0;
}
