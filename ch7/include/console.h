/*
 * Constants for console
 */

#ifndef _ORANGES_CONSOLE_H_
#define _ORANGES_CONSOLE_H_

/* see Figure 7.18 */
typedef struct s_console {
    u32    current_start_addr;  /* current pos of output */
    u32    original_addr;       /* video mem pos for current console */
    u32    v_mem_limit;         /* video mem size for current console */
    u32    cursor;              /* current cursor pos */
} CONSOLE;

/* screen constants */
#define SCR_UP      1      /* scroll up, show content before */
#define SCR_DN   -1      /* scroll down, show content after */
#define SCREEN_SIZE          (80 * 25)
#define SCREEN_WIDTH         80

#define DEFAULT_CHAR_COLOR      0x07    // black back, red fore

#endif /* _ORANGES_CONSOLE_H_ */