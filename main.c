#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_atoi.c"

const char *ARGP_PROGRAM_VERSION = "rcv 0.1.0";
const char *ARGP_PROGRAM_BUG_ADDRESS = "<mgilmour@maxgmr.ca>";
static char DOC[] = "An exceedingly simple C program used for quick conversion "
                    "between radicies.";
static char ARGS_DOC[] = "[INPUT_NUMBER]";

static struct argp_option OPTIONS[] = {
    {"dec", 'd', 0, 0, "(Default) output as a decimal number (radix 10)"},
    {"hex", 'h', 0, 0, "Output as a hexadecimal number (radix 16)"},
    {"oct", 'o', 0, 0, "Output as an octal number (radix 8)"},
    {"bin", 'b', 0, 0, "Output as a binary number (radix 2)"},
    {"no-newline", 'n', 0, 0, "Omit the line feed at the end of the output"},
    {0}};

struct args {
  enum { DEC_MODE, HEX_MODE, OCT_MODE, BIN_MODE } mode;
  bool is_no_newline;
  char *input;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct args *args = state->input;

  switch (key) {
  case 'd':
    args->mode = DEC_MODE;
    break;

  case 'h':
    args->mode = HEX_MODE;
    break;

  case 'o':
    args->mode = OCT_MODE;
    break;

  case 'b':
    args->mode = BIN_MODE;
    break;

  case 'n':
    args->is_no_newline = true;
    break;

  case ARGP_KEY_ARG:
    // Only one non-optional argument
    if (state->arg_num == 0) {
      // Duplicate the argument.
      args->input = strdup(arg);
      if (!args->input) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
      }
    } else {
      // Too many arguments
      argp_usage(state);
    }
    break;

  case ARGP_KEY_END:
    if (state->arg_num == 0) {
      // No input provided, read from stdin
      size_t len = 0;
      if (getline(&args->input, &len, stdin) == -1) {
        perror("getline");
        exit(EXIT_FAILURE);
      }
    }
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp ARGP = {OPTIONS, parse_opt, ARGS_DOC, DOC, 0, 0, 0};

int main(int argc, char *argv[]) {
  struct args args;

  args.mode = DEC_MODE;
  args.is_no_newline = false;
  args.input = NULL;

  argp_parse(&ARGP, argc, argv, 0, 0, &args);

  bool ok = true;
  int result = my_atoi(args.input, &ok);

  if (!ok) {
    fprintf(stderr, "Failed to parse input as number.\n");
    free(args.input);
    return 5;
  }

  fprintf(stdout, "%d", result);
  if (!args.is_no_newline) {
    fprintf(stdout, "\n");
  }
  free(args.input);
  return 0;
}
