#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

static fatal_t fatal_fn;
static fatal_arg_t fatal_arg;

void set_fatal_handler(fatal_t fn, fatal_arg_t arg)
{
    fatal_fn = fn;
    fatal_arg = arg;
}

void fatal(const char* fmt, ...)
{
    char msg[1024];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    msg[sizeof(msg) - 1] = '\0';

    if (fatal_fn != NULL) {
        fatal_fn(fatal_arg, msg);
    }

    fprintf(stderr, "fatal error: %s\n", msg);
    exit(EXIT_FAILURE);
}

