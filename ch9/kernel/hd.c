#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "hd.h"

PRIVATE void init_hd              ();
PRIVATE void hd_cmd_out           (struct hd_cmd* cmd);
PRIVATE int  waitfor              (int mask, int val, int timeout);
PRIVATE void interrupt_wait       ();
PRIVATE void hd_identify          (int drive);
PRIVATE void print_identify_info  (u16* hdinfo);

PRIVATE u8   hd_status;
PRIVATE u8   hdbuf[SECTOR_SIZE * 2];

// main function to send/recv and handle HD-related msg
PUBLIC void task_hd() {
    MESSAGE msg;
    init_hd();

    while (1) {
        send_recv(RECEIVE, ANY, &msg);

        int src = msg.source;
        switch(msg.type) {
            case DEV_OPEN:
                hd_identify(0);
                break;
            default:
                dump_msg("HD driver::unknown msg", &msg);
                spin("FS::main_loop (invalid msg.type");
                break;
        }

        send_recv(SEND, src, &msg);
    }
}

// setup hd_handler and hd_int
PRIVATE void init_hd() {
    // get the number of drives from BIOS data area
    u8* pNrDrives = (u8*)(0x475);
    printl("NrDrives:%d.\n", *pNrDrives);
    assert(*pNrDrives);

    put_irq_handler(AT_WINI_IRQ, hd_handler);
    enable_irq(CASCADE_IRQ);       // must enable this! HD is on the slave 8259
    enable_irq(AT_WINI_IRQ);
}

// get the disk info, after receiving DEV_OPEN msg
PRIVATE void hd_identify(int drive) {
    struct hd_cmd cmd;
    cmd.device = MAKE_DEVICE_REG(0, drive, 0);
    cmd.command = ATA_IDENTIFY;
    hd_cmd_out(&cmd);
    interrupt_wait();
    port_read(REG_DATA, hdbuf, SECTOR_SIZE);   // in kliba.asm

    print_identify_info((u16*)hdbuf);
}

// print the info retrived from ATA_IDENTIFY command to hd
PRIVATE void print_identify_info(u16* hdinfo) {
    int i, k;
    char s[64];

    struct iden_info_ascii {
        int idx;
        int len;
        char* desc;
    } iinfo[] = {{10, 20, "HD SN"},      // serial number in ASCII
                 {27, 40, "HD Model"}};  // model number in ASCII
    for (k = 0; k < sizeof(iinfo)/sizeof(iinfo[0]); k++) {
        char* p = (char*)&hdinfo[iinfo[k].idx];
        for (i = 0; i < iinfo[k].len / 2; i++) {
            s[i * 2 + 1] = *p++;
            s[i * 2] = *p++;
        }
        s[i * 2] = 0;
        printl("%s: %s\n", iinfo[k].desc, s);
    }

    int capabilities = hdinfo[49];
    printl("LBA supported: %s\n", 
            (capabilities & 0x0200) ? "Yes" : "No");
    int cmd_set_supported = hdinfo[83];
    printl("LBA48 supported: %s\n",
            (cmd_set_supported & 0x0400) ? "Yes" : "No");
    int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
    printl("HD size: %dMB\n", sectors * 512 / 1000000);
}

// send out a command to HD controller, see Table 9.1 and Figure 9.3
PRIVATE void hd_cmd_out(struct hd_cmd* cmd) {
    // check the status register, only send cmd when BSY bit is off
    if (!waitfor(STATUS_BSY, 0, HD_TIMEOUT)) {
        panic("hd error.");
    }

    // enable Interrupt Enable bit, see Figure 9.4
    out_byte(REG_DEV_CTRL, 0);
    // load values into the corresponding regs
    out_byte(REG_FEATURES, cmd->features);
    out_byte(REG_NSECTOR, cmd->count);
    out_byte(REG_LBA_LOW, cmd->lba_low);
    out_byte(REG_LBA_MID, cmd->lba_mid);
    out_byte(REG_LBA_HIGH, cmd->lba_high);
    out_byte(REG_DEVICE, cmd->device);
    // write command code into command reg
    out_byte(REG_CMD, cmd->command);    
}

// wait until a disk interrupt occurs
PRIVATE void interrupt_wait() {
    MESSAGE msg;
    send_recv(RECEIVE, INTERRUPT, &msg);
}

// wait for the REG_STATUS to be val after mask, within timeout
PRIVATE int waitfor(int mask, int val, int timeout) {
    int t = get_ticks();

    // if within timeout and the value is the same as val, return true
    while (((get_ticks() - t) * 1000 / HZ) < timeout) {
        if ((in_byte(REG_STATUS) & mask) == val) {
            return 1;
        }
    }
    
    return 0;
}

// hd int handler, will run on kernel level (ring 0)
PUBLIC void hd_handler(int irq) {
    /*
     * int will be cleared when:
     * - read status reg
     * - issue a reset
     * - write to command reg
     */
    hd_status = in_byte(REG_STATUS);   // clear the int

    inform_int(TASK_HD);    
}