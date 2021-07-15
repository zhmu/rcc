#ifndef SCAN_H
#define SCAN_H

enum TOKEN_TYPE {
    TT_EOF,
    TT_PLUS,
    TT_PLUS_ASSIGN,
    TT_PLUS_PLUS,
    TT_MINUS,
    TT_MINUS_ASSIGN,
    TT_MINUS_MINUS,
    TT_ARROW,
    TT_STAR,
    TT_STAR_ASSIGN,
    TT_SLASH,
    TT_SLASH_ASSIGN,
    TT_MODULO,
    TT_MODULO_ASSIGN,
    TT_AMP,
    TT_AMP_ASSIGN,
    TT_AMP_AMP,
    TT_CARET,
    TT_CARET_ASSIGN,
    TT_PIPE,
    TT_PIPE_ASSIGN,
    TT_PIPE_PIPE,
    TT_LITERAL_INT,
    TT_GT,
    TT_GE,
    TT_SHIFT_RIGHT,
    TT_SHIFT_RIGHT_ASSIGN,
    TT_LT,
    TT_LE,
    TT_SHIFT_LEFT,
    TT_SHIFT_LEFT_ASSIGN,
    TT_ELLIPSIS,
    TT_LBRACE,
    TT_RBRACE,
    TT_SEMICOLON,
    TT_NOT_EQUALS,
    TT_NOT,
    TT_TILDE,
    TT_EQUALS,
    TT_COMMA,
    TT_COLON,
    TT_LPAREN,
    TT_RPAREN,
    TT_LBRACKET,
    TT_RBRACKET,
    TT_DOT,
    TT_QMARK,
    TT_ASSIGN,
    /* Keywords */
    TT_K_AUTO,
    TT_K_BREAK,
    TT_K_CASE,
    TT_K_CHAR,
    TT_K_CONST,
    TT_K_CONTINUE,
    TT_K_DEFAULT,
    TT_K_DO,
    TT_K_DOUBLE,
    TT_K_ELSE,
    TT_K_ENUM,
    TT_K_EXTERN,
    TT_K_FLOAT,
    TT_K_FOR,
    TT_K_GOTO,
    TT_K_IF,
    TT_K_INT,
    TT_K_LONG,
    TT_K_REGISTER,
    TT_K_RETURN,
    TT_K_SHORT,
    TT_K_SIGNED,
    TT_K_SIZEOF,
    TT_K_STATIC,
    TT_K_STRUCT,
    TT_K_SWITCH,
    TT_K_TYPEDEF,
    TT_K_UNION,
    TT_K_UNSIGNED,
    TT_K_VOID,
    TT_K_VOLATILE,
    TT_K_WHILE,
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
