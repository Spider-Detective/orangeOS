/*
 * Constants for console
 */

#ifndef _ORANGES_CONSOLE_H_
#define _ORANGES_CONSOLE_H_

/* see Figure 7.18 */
typedef struct s_console {
    u32    crtc_start;     /* current pos of output */
    u32    orig;           /* video mem pos for current console */
    u32    con_size;       /* video mem size for current console */
    u32    cursor;         /* current cursor pos */
    int    is_full;
} CONSOLE;

/* screen constants */
#define SCR_UP      1      /* scroll up, show content before */
#define SCR_DN      -1      /* scroll down, show content after */
#define SCR_SIZE          (80 * 25)
#define SCR_WIDTH         80

#define DEFAULT_CHAR_COLOR      MAKE_COLOR(BLACK, WHITE)  /* defined in const.h */
#define GRAY_CHAR               (MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR                (MAKE_COLOR(BLUE, RED) | BRIGHT)

#endif /* _ORANGES_CONSOLE_H_ */