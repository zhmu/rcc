#include "scan.h"
#include <stdio.h>
#include <string.h>
#include "lib.h"

#define MAX_KEYWORD_LENGTH 32

static scan_t scan_fn;
static scan_arg_t scan_arg;
static int pending_char = 0;
static int line = 1;

struct KEYWORD_TABLE {
    const char* keyword;
    enum KEYWORD value;
};

static struct KEYWORD_TABLE keyword_table[] = {
    { "int", K_INT },
};

static int parse_digit(int c)
{
    if (c >= '0' && c <= '9') return c - '0';
    return -1;
}

static int parse_alpha(int c)
{
    if (c >= 'a' && c <= 'z') return c;
    return -1;
}

static int isdigit(int c)
{
    return parse_digit(c) >= 0;
}

static int isalpha(int c)
{
    return parse_alpha(c) >= 0;
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

static void parse_string(struct Token* t, char* s, size_t max_len)
{
    size_t pos = 0;
    while (pos < max_len - 1) {
        int c = next();
        int v = parse_alpha(c);
        if (v < 0) {
            scan_set_pending_char(c);
            s[pos] = '\0';
            return;
        }

        s[pos] = v;
        ++pos;
    }

    fatal("too many characters while parsing string");
}

static void parse_keyword(struct Token* t)
{
    t->type = TT_KEYWORD;

    char keyword[MAX_KEYWORD_LENGTH + 1];
    parse_string(t, keyword, sizeof(keyword));

    for (size_t n = 0; n < sizeof(keyword_table) / sizeof(keyword_table[0]); ++n) {
        if (strcmp(keyword, keyword_table[n].keyword) != 0)
            continue;
        t->value = keyword_table[n].value;
        return;
    }

    fatal("unrecognized keyword '%s'", keyword);
}

int scan(struct Token* t)
{
    int c = skip_whitespace();

    switch(c) {
        case EOF:
            t->type = TT_EOF;
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

            if (isalpha(c)) {
                scan_set_pending_char(c);
                parse_keyword(t);
                return 1;
            }

            fatal("unrecognized character '%c' on line %d", c, line);
    }
    return 1;
}
