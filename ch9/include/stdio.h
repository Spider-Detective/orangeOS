/*
 * I/O related definitions
 */

#define ASSERT
#ifdef  ASSERT
void assertion_failure(char* exp, char* file, char* base_file, int line);
#define assert(exp)  if (exp) ; \
                else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif
/* EXTERN is defined as "extern" except in global.c, see global.h */
#define EXTERN extern

#define STR_DEFAULT_LEN 1024

#define O_CREAT        1
#define O_RDWR         2

#define SEEK_SET       1
#define SEEK_CUR       2
#define SEEK_END       3

#define MAX_PATH       128

/* lib/open.c */
PUBLIC int open(const char* pathname, int flags);
PUBLIC int close(int fd);