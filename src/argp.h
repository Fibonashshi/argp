/* argp.h */
#ifndef ARGP_H
#define ARGP_H

typedef enum {
    ARGUMENT_FLAG,     // boolean
    ARGUMENT_OPTION,   // option with value
    ARGUMENT_POSITIONAL // positional
} ArgType;

typedef struct {
    const char *short_name;
    const char *long_name;
    const char *help;
    ArgType type;
    void *value;
} Arg;

typedef struct {
    Arg *args;
    int count;
    int capacity;
} Argp;

#ifdef __cplusplus
extern "C" {
#endif

Argp *argp_new(void);
void argp_add_argument(Argp *parser, const char *short_name, const char *long_name, ArgType type, void *value, const char *help);
int argp_parse(Argp *parser, int argc, char **argv);
void argp_print_help(Argp *parser, const char *prog_name);
void argp_free(Argp *parser);
static Arg *find_argument(Argp *parser, const char *arg);
#ifdef __cplusplus
}
#endif

#endif // ARGP_H
