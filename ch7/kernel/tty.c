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

        // make the cursor follows console's I/O
        disable_int();
        out_byte(CRTC_ADDR_REG, CURSOR_H);    // write into the addr reg port the reg you want to manipulate
        out_byte(CRTC_DATA_REG, ((disp_pos / 2) >> 8) & 0xFF);   // then write into the data reg port
        out_byte(CRTC_ADDR_REG, CURSOR_L);
        out_byte(CRTC_DATA_REG, (disp_pos / 2) & 0xFF);   // divided by 2 since each console position contains 2 bytes
        enable_int();

    } else {
        int raw_code = key & MASK_RAW;  // remove the non-functional key flags
        switch(raw_code) {
            case UP:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                    // scroll down the console from (0,0) to (0,15)
                    disable_int();
                    out_byte(CRTC_ADDR_REG, START_ADDR_H);   
                    out_byte(CRTC_DATA_REG, ((80 * 15) >> 8) & 0xFF);   
                    out_byte(CRTC_ADDR_REG, START_ADDR_L);
                    out_byte(CRTC_DATA_REG, (80 * 15) & 0xFF);   
                    enable_int(); 
                }
                break;
            case DOWN:
                if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {

                }
                break;
            default:
                break;
        }
    }
}