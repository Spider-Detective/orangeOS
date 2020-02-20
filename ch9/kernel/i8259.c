// setup 8259A for interrupt
#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "proc.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

// see 8259A assembly code in ch3/
PUBLIC void init_8259A() {
    out_byte(INT_M_CTL, 0x11);                      // Master 8259, ICW1
    out_byte(INT_S_CTL, 0x11);                      // Slave 8259, ICW1
    out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);       // Master 8259 int from 0x20, ICW2
    out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);       // Slave 8259 int from 0x28, ICW2
    out_byte(INT_M_CTLMASK, 0x4);                   // Master 8259, ICW3, IR2 -> slave
    out_byte(INT_S_CTLMASK, 0x2);                   // Slave 8259, ICW3, slave -> IR2
    out_byte(INT_M_CTLMASK, 0x1);                   // Master 8259, ICW4
    out_byte(INT_S_CTLMASK, 0x1);                   // Slave 8259, ICW4

    // out_byte(INT_M_CTLMASK, 0xFD);               // Master 8259, OCW1, enable keyboard int
    out_byte(INT_M_CTLMASK, 0xFF);                  // Master 8259, OCW1, block all ints
    out_byte(INT_S_CTLMASK, 0xFF);                  // Slave 8259, OCW1, block all ints

    int i;
    for (i = 0; i < NR_IRQ; i++) {
        irq_table[i] = spurious_irq;
    }

}

// simply printing the irq number when receive an int
PUBLIC void spurious_irq(int irq) {
    disp_str("spurious_irq: ");
    disp_int(irq);
    disp_str("\n");
}

// set handler for given irq
PUBLIC void put_irq_handler(int irq, irq_handler handler) {
    disable_irq(irq);
    irq_table[irq] = handler;
}