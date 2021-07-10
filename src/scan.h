#ifndef SCAN_H
#define SCAN_H

enum TOKEN_TYPE {
    TT_EOF,
    TT_PLUS,
    TT_MINUS,
    TT_STAR,
    TT_SLASH,
    TT_LITERAL_INT,
    TT_KEYWORD,
};

enum KEYWORD {
    K_INT,
};

struct Token {
    enum TOKEN_TYPE type;
    int value;
};

typedef void* scan_arg_t;
typedef int (*scan_t)(scan_arg_t);

#ifdef __cplusplus
extern "C" {
#endif

void scan_set_fn(scan_t, scan_arg_t);
void scan_set_pending_char(int ch);
int scan(struct Token* t);

#ifdef __cplusplus
}
#endif

#endif
