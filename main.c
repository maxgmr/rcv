#include <argp.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_atoi.c"

const char *argp_program_version = "rcv 0.1.0";
const char *argp_program_bug_address = "<mgilmour@maxgmr.ca>";
static char doc[] = "An exceedingly simple C program used for quick conversion\
                    between radicies.";
static char args_doc[] = "[INPUT_NUMBER]";

static struct argp_option options[] = {
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

void replace_int_prefix_char(char *s, char original, char new) {
  int len = (int)strlen(s);
  if ((len > 1) && (s[0] == original) && isdigit(s[1])) {
    s[0] = new;
  }
  return;
}

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
      replace_int_prefix_char(args->input, 'N', '-');
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

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {
  struct args args;

  args.mode = DEC_MODE;
  args.is_no_newline = false;
  args.input = NULL;

  int i;
  for (i = 0; i < argc; ++i) {
    replace_int_prefix_char(argv[i], '-', 'N');
  }

  argp_parse(&argp, argc, argv, 0, 0, &args);

  bool ok = true;
  int result = my_atoi(args.input, &ok);

  if (!ok) {
    fprintf(stderr, "Failed to parse input as number.\n");
    free(args.input);
    return 5;
  }

  switch (args.mode) {
  case (HEX_MODE):
    fprintf(stdout, "0x%X", result);
    break;
  case (OCT_MODE):
    fprintf(stdout, "0o%o", result);
    break;
  case (BIN_MODE):
    fprintf(stdout, "TODO");
    break;
  default:
    fprintf(stdout, "%d", result);
    break;
  }
  if (!args.is_no_newline) {
    fprintf(stdout, "\n");
  }
  free(args.input);
  return 0;
}
