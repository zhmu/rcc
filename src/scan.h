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
    K_AUTO,
    K_BREAK,
    K_CASE,
    K_CHAR,
    K_CONST,
    K_CONTINUE,
    K_DEFAULT,
    K_DO,
    K_DOUBLE,
    K_ELSE,
    K_ENUM,
    K_EXTERN,
    K_FLOAT,
    K_FOR,
    K_GOTO,
    K_IF,
    K_INT,
    K_LONG,
    K_REGISTER,
    K_RETURN,
    K_SHORT,
    K_SIGNED,
    K_SIZEOF,
    K_STATIC,
    K_STRUCT,
    K_SWITCH,
    K_TYPEDEF,
    K_UNION,
    K_UNSIGNED,
    K_VOID,
    K_VOLATILE,
    K_WHILE,
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
