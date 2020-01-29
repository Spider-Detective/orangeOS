/*
 * Constants for console
 */

#ifndef _ORANGES_CONSOLE_H_
#define _ORANGES_CONSOLE_H_

typedef struct s_console {
    u32    current_start_addr;  /* current pos of output */
    u32    original_addr;       /* video mem pos for current console */
    u32    v_mem_limit;         /* video mem size for current console */
    u32    cursor;              /* current cursor pos */
} CONSOLE;

#define DEFAULT_CHAR_COLOR      0x07    // black back, red fore

#endif /* _ORANGES_CONSOLE_H_ */