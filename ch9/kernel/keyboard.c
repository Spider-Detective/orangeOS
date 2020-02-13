#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"
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

PRIVATE int            caps_lock;
PRIVATE int            num_lock;
PRIVATE int            scroll_lock;

PRIVATE u8             get_byte_from_kbuf();
PRIVATE void           set_leds();
PRIVATE void           kb_wait();
PRIVATE void           kb_ack();

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

    // TODO:
    shift_l = shift_r = 0;
    alt_l = alt_r = 0;
    ctrl_l = ctrl_r = 0;

    // set numLock LED to be on
    caps_lock = 0;
    num_lock = 1;
    scroll_lock = 0;
    set_leds();

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

PUBLIC void keyboard_read(TTY* p_tty) {
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

            int caps = shift_l || shift_r;
            // if capsLock is on, reverse the effect of pressing shift key
            if (caps_lock) {
                if ((keyrow[0] >= 'a') && (keyrow[0] <= 'z')) {
                    caps = !caps;
                }
            }
            if (caps) {       // if shift key is being pressed, go to column 1
                column = 1;
            }
            if (code_with_E0) {
                column = 2;
            }

            key = keyrow[column];

            switch(key) {
                case SHIFT_L:
                    shift_l = make;         // only true when being pressed (output make code, not break code)
                    break;
                case SHIFT_R:
                    shift_r = make;         
                    break;
                case CTRL_L:
                    ctrl_l = make;         
                    break;
                case CTRL_R:
                    ctrl_r = make;         
                    break;
                case ALT_L:
                    alt_l = make;         
                    break;
                case ALT_R:
                    alt_r = make;         
                    break;
                case CAPS_LOCK:
                    if (make) {
                        caps_lock   = !caps_lock;
                        set_leds();
                    }
                    break;
                case NUM_LOCK:
                    if (make) {
                        num_lock    = !num_lock;
                        set_leds();
                    }
                    break;
                case SCROLL_LOCK:
                    if (make) {
                        scroll_lock = !scroll_lock;
                        set_leds();
                    }
                    break;
                default:                    
                    break;
            }
            if (make) {    // ignore break code
                int pad = 0;

				/* pad: accompany keyboard, convert into normal keys */
				if ((key >= PAD_SLASH) && (key <= PAD_9)) {
					pad = 1;
					switch(key) {
					case PAD_SLASH:
						key = '/';
						break;
					case PAD_STAR:
						key = '*';
						break;
					case PAD_MINUS:
						key = '-';
						break;
					case PAD_PLUS:
						key = '+';
						break;
					case PAD_ENTER:
						key = ENTER;
						break;
					default:
						if (num_lock &&
						    (key >= PAD_0) &&
						    (key <= PAD_9)) {
							key = key - PAD_0 + '0';
						}
						else if (num_lock &&
							 (key == PAD_DOT)) {
							key = '.';
						}
						else{
							switch(key) {
							case PAD_HOME:
								key = HOME;
								break;
							case PAD_END:
								key = END;
								break;
							case PAD_PAGEUP:
								key = PAGEUP;
								break;
							case PAD_PAGEDOWN:
								key = PAGEDOWN;
								break;
							case PAD_INS:
								key = INSERT;
								break;
							case PAD_UP:
								key = UP;
								break;
							case PAD_DOWN:
								key = DOWN;
								break;
							case PAD_LEFT:
								key = LEFT;
								break;
							case PAD_RIGHT:
								key = RIGHT;
								break;
							case PAD_DOT:
								key = DELETE;
								break;
							default:
								break;
							}
						}
						break;
					}
                }
                key |= shift_l ? FLAG_SHIFT_L   : 0;
                key |= shift_r ? FLAG_SHIFT_R   : 0;
                key |= ctrl_l  ? FLAG_CTRL_L    : 0;
                key |= ctrl_r  ? FLAG_CTRL_R    : 0;
                key |= alt_l   ? FLAG_ALT_L     : 0;
                key |= alt_r   ? FLAG_ALT_R     : 0;
                key |= pad     ? FLAG_PAD       : 0;

                in_process(p_tty, key);
            }
        }
    }
}

PRIVATE u8 get_byte_from_kbuf() {
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

// read 0x64 CMD port, until input buffer of 8042 to be 0 (so as to write data into it)
PRIVATE void kb_wait() {
    u8 kb_stat;

    do {
        kb_stat = in_byte(KB_CMD);
    } while (kb_stat & 0x02);     // first bit (not 0-th byte)
}

// read 0x60 DATA port until receive ACK from keyboard
PRIVATE void kb_ack() {
    u8 kb_read;

    do {
        kb_read = in_byte(KB_DATA);
    } while (kb_read != KB_ACK);
}

PRIVATE void set_leds() {
    u8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;  // see Figure 7.22

    kb_wait();
    out_byte(KB_DATA, LED_CODE);  // ask to set LED
    kb_ack();

    kb_wait();
    out_byte(KB_DATA, leds);      // set LED
    kb_ack();
}