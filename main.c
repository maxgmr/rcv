#include <argp.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_atoi.c"

const char *argp_program_version = "rcv 0.2.0";
const char *argp_program_bug_address = "<mgilmour@maxgmr.ca>";
static char doc[] = "An exceedingly simple C program used for quick conversion\
                    between radicies.";
static char args_doc[] = "[INPUT_NUMBER]";

static struct argp_option options[] = {
    {"dec", 'i', 0, 0,
     "(Default) output as a signed decimal number (radix 10)"},
    {"dec", 'u', 0, 0, "Output as an unsigned decimal number (radix 10)"},
    {"hex", 'x', 0, 0, "Output as a hexadecimal number (radix 16)"},
    {"oct", 'o', 0, 0, "Output as an octal number (radix 8)"},
    {"bin", 'b', 0, 0, "Output as a binary number (radix 2)"},
    {"no-newline", 'n', 0, 0, "Omit the line feed at the end of the output"},
    {"no-prefix", 'p', 0, 0, "Omit the prefix for hexadecimal, octal, and binary output"},
    {"coct-prefix", 'c', 0, 0, "Use a C-style octal prefix (leading 0) instead of a\
        Rust-style octal prefix (leading 0o)"},
    {0}};

struct args {
  enum { I_MODE, U_MODE, HEX_MODE, OCT_MODE, BIN_MODE } mode;
  bool is_no_newline;
  bool is_no_prefix;
  bool is_coct_prefix;
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
  case 'i':
    args->mode = I_MODE;
    break;

  case 'u':
    args->mode = U_MODE;
    break;

  case 'x':
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

  case 'p':
    args->is_no_prefix = true;
    break;

  case 'c':
    args->is_coct_prefix = true;
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

void binary_print(unsigned a) {
  if (a > 1) {
    binary_print(a / 2);
  }
  printf("%d", a % 2);
}

int main(int argc, char *argv[]) {
  struct args args;

  // Default args
  args.mode = I_MODE;
  args.is_no_newline = false;
  args.is_no_prefix = false;
  args.is_coct_prefix = false;
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
  case (U_MODE):
    fprintf(stdout, "%u", result);
    break;
  case (HEX_MODE):
    if (!args.is_no_prefix) {
        fprintf(stdout, "0x");
    }
    fprintf(stdout, "%X", result);
    break;
  case (OCT_MODE):
    if (!args.is_no_prefix) {
        if (args.is_coct_prefix) {
            fprintf(stdout, "0");
        } else {
            fprintf(stdout, "0o");
        }
    }
    fprintf(stdout, "%o", result);
    break;
  case (BIN_MODE):
    if (!args.is_no_prefix) {
        fprintf(stdout, "0b");
    }
    binary_print(result);
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
