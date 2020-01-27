#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT       kb_in;

PRIVATE int            code_with_E0;
PRIVATE int            shift_l;           // left shift key's status
PRIVATE int            shift_r;           // right shift key's status
PRIVATE int            alt_l;           
PRIVATE int            alt_r;          
PRIVATE int            ctrl_l;           
PRIVATE int            ctrl_r;           
PRIVATE int            caps_lock;           
PRIVATE int            num_lock;           
PRIVATE int            scroll_lock;           
PRIVATE int            column;            // col no. in keymap

PRIVATE u8 get_byte_from_kbuf();

PUBLIC void keyboard_handler(int irq) {
    u8 scan_code = in_byte(KB_DATA);      // see spec for 8042 and 8048, Table 7.1
    if (kb_in.count < KB_IN_BYTES) {
        *(kb_in.p_head) = scan_code;
        kb_in.p_head++;
        if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
            kb_in.p_head = kb_in.buf;
        }
        kb_in.count++;
    }
}

PUBLIC void init_keyboard() {
    // initialize the circular buffer
    kb_in.count = 0;
    kb_in.p_head = kb_in.p_tail = kb_in.buf;

    shift_l = shift_r = 0;
    alt_l = alt_r = 0;
    ctrl_l = ctrl_r = 0;
    code_with_E0 = 0;

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

PUBLIC void keyboard_read() {
    u8     scan_code;
    int    make;
    u32    key = 0;      // use a 32-bit to store the key (code with flags)

    u32*   keyrow;       // pointing to a row in keymap

    if (kb_in.count > 0) {
        code_with_E0 = 0;
        scan_code = get_byte_from_kbuf();

        if (scan_code == 0xE1) {    // handle button "pause"
            int i;
            u8 pausebrk_scode[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};    // see Table 7.2 for button "pause"
            int is_pausebreak = 1;
            for (i = 1; i < 6; i++) {
                if (get_byte_from_kbuf() != pausebrk_scode[i]) {
                    is_pausebreak = 0;
                    break;
                }
            }
            if (is_pausebreak) {
                key = PAUSEBREAK;
            }
        } else if (scan_code == 0xE0) {  // handle button "printscreen"
            scan_code = get_byte_from_kbuf();

            // "Printscreen" is pressed
            if (scan_code == 0x2A) {
                if (get_byte_from_kbuf() == 0xE0) {
                    if (get_byte_from_kbuf() == 0x37) {
                        key = PRINTSCREEN;
                        make = 1;
                    }
                }
            }

            // "Printscreen" is released
            if (scan_code == 0xB7) {
                if (get_byte_from_kbuf() == 0xE0) {
                    if (get_byte_from_kbuf() == 0xAA) {
                        key = PRINTSCREEN;
                        make = 0;
                    }
                }
            }

            // if not "printscreen", continue to the following if check, with scan_code having the value right folloeing E0
            if (key == 0) {
                code_with_E0 = 1;
            }
        } 
        
        // handle all other buttons, scan_code is non-E1/E0, or it is the value right following E1 and E0
        if((key != PAUSEBREAK) && (key != PRINTSCREEN)) {
            /*
             * Workflow:
             * If shift_l or shift_r is true, that means only received make code of shift key => shift key is being pressed
             * Then when another make code is received, go to column 1 of the keymap
             */
            make = (scan_code & FLAG_BREAK) ? FALSE : TRUE;   // mask_code | 0x80 == break_code, see keymap.h
            
            keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];  // go to the row

            column = 0;
            if (shift_l || shift_r) {       // if shift key is being pressed, go to column 1
                column = 1;
            }
            if (code_with_E0) {
                column = 2;
                code_with_E0 = 0;
            }

            key = keyrow[column];

            switch(key) {
                case SHIFT_L:
                    shift_l = make;         // only true when being pressed (output make code, not break code)
                    key = 0;
                    break;
                case SHIFT_R:
                    shift_r = make;         
                    key = 0;
                    break;
                case CTRL_L:
                    ctrl_l = make;         
                    key = 0;
                    break;
                case CTRL_R:
                    ctrl_r = make;         
                    key = 0;
                    break;
                case ALT_L:
                    alt_l = make;         
                    key = 0;
                    break;
                case ALT_R:
                    alt_r = make;         
                    key = 0;
                    break;
                default:                    
                    break;
            }
            if (make) {    // ignore break code
                key |= shift_l ? FLAG_SHIFT_L   : 0;
                key |= shift_r ? FLAG_SHIFT_R   : 0;
                key |= shift_l ? FLAG_SHIFT_L   : 0;
                key |= shift_r ? FLAG_SHIFT_R   : 0;
                key |= shift_l ? FLAG_SHIFT_L   : 0;
                key |= shift_r ? FLAG_SHIFT_R   : 0;

                in_process(key);
            }
        }
    }
}

PUBLIC u8 get_byte_from_kbuf() {
    u8 scan_code;

    while (kb_in.count <= 0) {}         //sanity check, wait if no next byte
    
    disable_int();
    scan_code = *(kb_in.p_tail);
    kb_in.p_tail++;
    if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
        kb_in.p_tail = kb_in.buf;
    }
    kb_in.count--;
    enable_int();

    return scan_code;
}