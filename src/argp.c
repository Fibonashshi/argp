/* argp.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argp.h"

#define ARG_MATCH(s, a) ((s) && (a) && strcmp((s), (a)) == 0)

static void ensure_capacity(Argp *parser) {
    if (parser->count >= parser->capacity) {
        parser->capacity = parser->capacity ? parser->capacity * 2 : 8;
        void *tmp = realloc(parser->args, sizeof(Arg) * parser->capacity);
        if (!tmp) {
            fprintf(stderr, "Memory allocation failed during ensure_capacity\n");
            exit(EXIT_FAILURE);
        }
        parser->args = tmp;
    }
}

Argp *argp_new(void) {
    Argp *parser = malloc(sizeof(Argp));
    if (!parser) {
        fprintf(stderr, "Memory allocation failed during parser creation\n");
        exit(EXIT_FAILURE);
    }
    parser->args = NULL;
    parser->count = 0;
    parser->capacity = 0;
    return parser;
}

void argp_add_argument(Argp *parser, const char *short_name, const char *long_name, ArgType type, void *value, const char *help) {
    ensure_capacity(parser);
    parser->args[parser->count++] = (Arg){ short_name, long_name, help, type, value };
}

static Arg *find_argument(Argp *parser, const char *arg) {
    for (int i = 0; i < parser->count; ++i) {
        const char *s = parser->args[i].short_name;
        const char *l = parser->args[i].long_name;

        if ((s && strcmp(arg, s) == 0) || (l && strcmp(arg, l) == 0)) {
            return &parser->args[i];
        }
    }
    return NULL;
}


int argp_parse(Argp *parser, int argc, char **argv) {
    int positional_assigned = 0;

    for (int i = 1; i < argc; ++i) {
        char *arg = argv[i];

        if (arg[0] == '-') {
            // handle --option=value
            char *eq = strchr(arg, '=');
            if (eq && arg[1] == '-') {
                *eq = '\0';
                Arg *a = find_argument(parser, arg);
                *eq = '='; // restore string

                if (!a) {
                    fprintf(stderr, "unknown option: %s\n", arg);
                    return -1;
                }

                if (a->type != ARGUMENT_OPTION) {
                    fprintf(stderr, "option %s does not take a value\n", arg);
                    return -1;
                }

                *(char **)(a->value) = eq + 1;
                continue;
            }

            Arg *a = find_argument(parser, arg);
            if (!a) {
                fprintf(stderr, "unknown option: %s\n", arg);
                return -1;
            }

            if (a->type == ARGUMENT_FLAG) {
                *(int *)(a->value) = 1;
            } else if (a->type == ARGUMENT_OPTION) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "no value provided for %s\n", arg);
                    return -1;
                }
                *(char **)(a->value) = argv[++i];
            }
        } else {
            // Positional argument
            int matched = 0;
            for (int j = 0; j < parser->count; ++j) {
                if (parser->args[j].type == ARGUMENT_POSITIONAL && !(*(char **)parser->args[j].value)) {
                    *(char **)parser->args[j].value = arg;
                    matched = 1;
                    positional_assigned++;
                    break;
                }
            }
            if (!matched) {
                fprintf(stderr, "unexpected positional argument: %s\n", arg);
                return -1;
            }
        }
    }

    return 0;
}

void argp_print_help(Argp *parser, const char *prog_name) {
    printf("usage: %s [OPTIONS] [ARGUMENTS]\n", prog_name);
    for (int i = 0; i < parser->count; ++i) {
        Arg *a = &parser->args[i];
        const char *s = a->short_name ? a->short_name : "";
        const char *l = a->long_name ? a->long_name : "";
        printf("  %-4s %-20s %s\n", s, l, a->help ? a->help : "");
    }
}

void argp_free(Argp *parser) {
    if (parser) {
        free(parser->args);
        free(parser);
    }
}
