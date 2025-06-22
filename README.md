# argp
Modest argument parser for C programming language.

## ✨ Features

- Supports:
  - Boolean flags (e.g., `--verbose`)
  - Options with values (e.g., `--output file.txt`, `--output=file.txt`)
  - Positional arguments (e.g., `filename.txt`)
- Automatically handles `--help`
- No external libraries or dependencies
- Designed to be embedded in any C project

## 🛠️ Compilation
To successfully compile the project, make sure to build your main source file (e.g. example.c) together with argp.c. The argp.c file contains the implementation of the interface declared in argp.h, and must be compiled as part of the final binary — it is not a precompiled library.

A minimal compilation command using clang:

```bash
clang example.c argp.c -o program
```
If you're using gcc, the command is essentially the same:

```bash
gcc example.c argp.c -o program
```
Ensure that both argp.c and argp.h are in the same directory as your main source file. If they reside elsewhere, remember to adjust include paths accordingly:

```bash
clang -I./include example.c path/to/argp.c -o program
```
> 📎 Note: argp.c includes argp.h, so the header file must be visible during compilation. If you're planning to use this module across multiple projects or scale it further, consider setting up a simple Makefile for better maintainability.



## 🧪 Basic usage with all argument types
### 🧱 Example code

```c
#include <stdio.h>
#include <stdlib.h>

#include "argp.h"

int main(int argc, char *argv[]) {
    int verbose = 0;          // verbose flag
    char *output_file = NULL; // output option
    char *input_file = NULL;  // positional argument for input file

    Argp *parser = argp_new(); // Initialize parser
    
    // Used three possible argument types: flag, option and positional
    argp_add_argument(parser, "-v", "--verbose", ARGUMENT_FLAG, &verbose, "Enable verbose output");
    argp_add_argument(parser, "-o", "--output", ARGUMENT_OPTION, &output_file, "Output file name");
    argp_add_argument(parser, NULL, NULL, ARGUMENT_POSITIONAL, &input_file, "Input file");
    
    // Parsing
    if (argp_parse(parser, argc, argv) != 0) {
        // If error occurs, show an error
        argp_print_help(parser, argv[0]);
        argp_free(parser);
        return EXIT_FAILURE;
    }

    // Check values after parsing
    printf("Verbose: %s\n", verbose ? "ON" : "OFF"); // verbose (flag)
    
    if (output_file) // output file (option)
        printf("Output file: %s\n", output_file);
    else
        printf("Output file not specified\n");

    if (input_file) // input file (positional)
        printf("Input file: %s\n", input_file);
    else {
        fprintf(stderr, "Input file is required!\n");
        argp_print_help(parser, argv[0]);
        argp_free(parser);
        return EXIT_FAILURE;
    }

    argp_free(parser);
    return EXIT_SUCCESS;
}
```

### 🧪 Input and output after compiling:
```bash
$ ./program program.c --verbose -o output.txt
Verbose: ON
Output file: output.txt
Input file: program.c
$ 
```

## ⚙️ Passing Data to Arguments

### 📌 Positional Arguments

Positional arguments are values passed to the program **without a flag**.  
Their meaning is determined solely by **their order** in the command line.

Example:
```bash
$ ./program input.txt
```
### 🧩 Option arguments
Optional arguments can be provided in two equivalent forms, depending on user preference or scripting needs:

- **Separate form**: `--output output.txt`
- **Inline form with equals sign**: `--output=output.txt`

Both forms are parsed identically and can be used interchangeably.

This applies to all long options that expect a value, such as:

- `--config=config.yaml` or `--config config.yaml`
- `--log-level=debug` or `--log-level debug`

Short options (e.g., `-o`) typically require a space-separated value (`-o output.txt`), and do not support the equals sign syntax.

> ℹ Note: Boolean flags like `--verbose` do not take a value — their mere presence implies activation (i.e., true).

### 🎛️ Boolean Flags

Flags are special arguments that **do not accept a value** — their presence alone toggles a boolean `true`. So syntax like `--verbose=true` is not supported — simply use `--verbose`.

For example:

```bash
--verbose
--dry-run
```  
