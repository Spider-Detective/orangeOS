#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"

PUBLIC void task_tty() {
    while (1) {
        keyboard_read();
    }
}

PUBLIC void in_process(u32 key) {
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT)) {   // FLAG_EXT means it is not a printable char
        output[0] = key & 0xFF;
        disp_str(output);
    }
}