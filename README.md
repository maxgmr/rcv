# rcv

An exceedingly simple C program used for quick conversion between radicies.

## Usage

`rcv [OPTION...] [INPUT_NUMBER]`

## Options

* `-b, --bin`: Output as a binary number (radix 2)
* `-c, --coct-prefix`: Use a C-style octal prefix (leading 0) instead of a Rust-style octal prefix (leading 0o)
* `-i --dec`: (Default) output as a signed decimal number (radix 10)
* `-n, --no-newline`: Omit the line feed at the end of the output
* `-o, --oct`: Output as an octal number (radix 8)
* `-p, --no-prefix`: Omit the prefix for hexadecimal, octal, and binary output
* `-u, --dec`: Output as an unsigned decimal number (radix 10)
* `-x, --hex`: Output as a hexadecimal number (radix 16)
* `-?, --help`: Display the help page
* `-V, --version`: Print program version
