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
