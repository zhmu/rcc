#include "scan.h"
#include <stdio.h>
#include "lib.h"

#define MAX_KEYWORD_LENGTH 32

static scan_t scan_fn;
static scan_arg_t scan_arg;
static int pending_char = 0;
static int line = 1;

struct KEYWORD_TABLE {
    const char* keyword;
    enum TOKEN_TYPE type;
};

static struct KEYWORD_TABLE keyword_table[] = {
    { "auto", TT_K_AUTO },
    { "break", TT_K_BREAK },
    { "case", TT_K_CASE },
    { "char", TT_K_CHAR },
    { "const", TT_K_CONST },
    { "continue", TT_K_CONTINUE },
    { "default", TT_K_DEFAULT },
    { "do", TT_K_DO },
    { "double", TT_K_DOUBLE },
    { "else", TT_K_ELSE },
    { "enum", TT_K_ENUM },
    { "extern", TT_K_EXTERN },
    { "float", TT_K_FLOAT },
    { "for", TT_K_FOR },
    { "goto", TT_K_GOTO },
    { "if", TT_K_IF },
    { "int", TT_K_INT },
    { "long", TT_K_LONG },
    { "register", TT_K_REGISTER },
    { "return", TT_K_RETURN },
    { "short", TT_K_SHORT },
    { "signed", TT_K_SIGNED },
    { "sizeof", TT_K_SIZEOF },
    { "static", TT_K_STATIC },
    { "struct", TT_K_STRUCT },
    { "switch", TT_K_SWITCH },
    { "typedef", TT_K_TYPEDEF },
    { "union", TT_K_UNION },
    { "unsigned", TT_K_UNSIGNED },
    { "void", TT_K_VOID },
    { "volatile", TT_K_VOLATILE },
    { "while", TT_K_WHILE },
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

static int try(int v)
{
    int c = next();
    if (c == v)
        return 1;
    scan_set_pending_char(c);
    return 0;
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

static int try_number(int c, struct Token* t)
{
    if (!isdigit(c))
        return 0;
    scan_set_pending_char(c);
    parse_int(t);
    return 1;
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
    char keyword[MAX_KEYWORD_LENGTH + 1];
    parse_string(t, keyword, sizeof(keyword));

    for (size_t n = 0; n < sizeof(keyword_table) / sizeof(keyword_table[0]); ++n) {
        if (strcmp(keyword, keyword_table[n].keyword) != 0)
            continue;
        t->type = keyword_table[n].type;
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
            if (try('=')) {
                t->type = TT_PLUS_ASSIGN;
            } else if (try('+')) {
                t->type = TT_PLUS_PLUS;
            } else {
                t->type = TT_PLUS;
            }
            break;
        case '-':
            if (try('=')) {
                t->type = TT_MINUS_ASSIGN;
            } else if (try('-')) {
                t->type = TT_MINUS_MINUS;
            } else if (try('>')) {
                t->type = TT_ARROW;
            } else {
                t->type = TT_MINUS;
                // TODO numbers
            }
            break;
        case '*':
            if (try('=')) {
                t->type = TT_STAR_ASSIGN;
            } else {
                t->type = TT_STAR;
            }
            break;
        case '/':
            if (try('=')) {
                t->type = TT_SLASH_ASSIGN;
            } else {
                t->type = TT_SLASH;
            }
            break;
        case '>':
            if (try('>')) {
                if (try('=')) {
                    t->type = TT_SHIFT_RIGHT_ASSIGN;
                } else {
                    t->type = TT_SHIFT_RIGHT;
                }
            } else if (try('=')) {
                t->type = TT_GE;
            } else {
                t->type = TT_GT;
            }
            break;
        case '<':
            if (try('<')) {
                if (try('=')) {
                    t->type = TT_SHIFT_LEFT_ASSIGN;
                } else {
                    t->type = TT_SHIFT_LEFT;
                }
            } else if (try('=')) {
                t->type = TT_LE;
            } else {
                t->type = TT_LT;
            }
            break;
        case '.':
            if (try('.')) {
                if (try('.')) {
                    t->type = TT_ELLIPSIS;
                } else {
                    fatal("unrecognized character '%c' on line %d after '..'", c, line);
                }
            } else {
                t->type = TT_DOT;
            }
            break;
        case '%':
            if (try('=')) {
                t->type = TT_MODULO_ASSIGN;
            } else {
                t->type = TT_MODULO;
            }
            break;
        case '&':
            if (try('=')) {
                t->type = TT_AMP_ASSIGN;
            } else if (try('&')) {
                t->type = TT_AMP_AMP;
            } else {
                t->type = TT_AMP;
            }
            break;
        case '^':
            if (try('=')) {
                t->type = TT_CARET_ASSIGN;
            } else {
                t->type = TT_CARET;
            }
            break;
        case '|':
            if (try('=')) {
                t->type = TT_PIPE_ASSIGN;
            } else if (try('|')) {
                t->type = TT_PIPE_PIPE;
            } else {
                t->type = TT_PIPE;
            }
            break;
        case '{':
            t->type = TT_LBRACE;
            break;
        case '}':
            t->type = TT_RBRACE;
            break;
        case ';':
            t->type = TT_SEMICOLON;
            break;
        case '!':
            if (try('=')) {
                t->type = TT_NOT_EQUALS;
            } else {
                t->type = TT_NOT;
            }
            break;
        case '~':
            t->type = TT_TILDE;
            break;
        case '=':
            if (try('=')) {
                t->type = TT_ASSIGN;
            } else {
                t->type = TT_EQUALS;
            }
            break;
        case ',':
            t->type = TT_COMMA;
            break;
        case ':':
            t->type = TT_COLON;
            break;
        case '(':
            t->type = TT_LPAREN;
            break;
        case ')':
            t->type = TT_RPAREN;
            break;
        case '[':
            t->type = TT_LBRACKET;
            break;
        case ']':
            t->type = TT_RBRACKET;
            break;
        case '?':
            t->type = TT_QMARK;
            break;
        default:
            if (try_number(c, t)) return 1;

            if (isalpha(c)) {
                scan_set_pending_char(c);
                parse_keyword(t);
                return 1;
            }

            fatal("unrecognized character '%c' on line %d", c, line);
    }
    return 1;
}
