/*
 * Function prototypes for string-related functions
 */

PUBLIC  void* memcpy(void* p_dst, void* p_src, int size);
PUBLIC  void  memset(void* p_dst, char ch, int size);
PUBLIC  int   strlen(char* p_str);

/* only used in kernel, since the segments addr is based on 0 */
#define phys_copy     memcpy
#define phys_set      memset