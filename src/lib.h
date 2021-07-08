#ifndef LIB_H
#define LIB_H

typedef void* fatal_arg_t;
typedef void (*fatal_t)(fatal_arg_t, const char* msg);

#ifdef __cplusplus
extern "C" {
#endif
void set_fatal_handler(fatal_t fn, fatal_arg_t arg);
void fatal(const char* msg, ...);

#ifdef __cplusplus
}
#endif

#endif
