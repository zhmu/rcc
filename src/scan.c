#include "scan.h"
#include <stdio.h>
#include "lib.h"

static scan_t scan_fn;
static scan_arg_t scan_arg;
static int pending_char = 0;
static int line = 1;

static int parse_digit(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    return -1;
}

static int isdigit(int c)
{
    return parse_digit(c) >= 0;
}

void scan_set_fn(scan_t fn, scan_arg_t arg)
{
    scan_fn = fn;
    scan_arg = arg;
}

void scan_set_pending_char(int ch)
{
    pending_char = ch;
}

static int next()
{
    int c;

    if (pending_char) {
        c = pending_char;
        pending_char = 0;
        return c;
    }

    c = scan_fn(scan_arg);
    if (c == '\n') {
        ++line;
    }
    return c;
}

static int skip_whitespace()
{
    int c = next();

    while(c == ' ' || c == '\t' || c == '\n' || c == '\r')
        c = next();
    return c;
}

static void parse_int(struct Token* t)
{
    t->type = TT_LITERAL_INT;
    t->value = 0;

    while (1) {
        int c = next();
        int d = parse_digit(c);
        if (d < 0) {
            scan_set_pending_char(c);
            break;
        }

        t->value = t->value * 10 + d;
    }
}

int scan(struct Token* t)
{
    int c = skip_whitespace();

    switch(c) {
        case EOF:
            return 0;
        case '+':
            t->type = TT_PLUS;
            break;
        case '-':
            t->type = TT_MINUS;
            break;
        case '*':
            t->type = TT_STAR;
            break;
        case '/':
            t->type = TT_SLASH;
            break;
        default:
            if (isdigit(c)) {
                scan_set_pending_char(c);
                parse_int(t);
                return 1;
            }

            fatal("unrecognized character '%c' on line %d\n", c, line);
    }
    return 1;
}
