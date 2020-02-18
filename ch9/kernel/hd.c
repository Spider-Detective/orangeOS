#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "hd.h"

PRIVATE void init_hd              ();
PRIVATE void hd_open              (int device);
PRIVATE void hd_cmd_out           (struct hd_cmd* cmd);
PRIVATE void get_part_table       (int drive, int sect_nr, struct part_ent* entry);
PRIVATE void partition            (int device, int style);
PRIVATE void print_hdinfo         (struct hd_info* hdi);
PRIVATE int  waitfor              (int mask, int val, int timeout);
PRIVATE void interrupt_wait       ();
PRIVATE void hd_identify          (int drive);
PRIVATE void print_identify_info  (u16* hdinfo);

PRIVATE u8              hd_status;
PRIVATE u8              hdbuf[SECTOR_SIZE * 2];
PRIVATE struct hd_info  hd_info[1];

// get the drive number (0 or 1) from given device number, see Figure 9.9
#define DRV_OF_DEV(dev) (dev <= MAX_PRIM ? \
                         dev / NR_PRIM_PER_DRIVE : \
                         (dev - MINOR_hd1a) / NR_SUB_PER_DRIVE)

// main function to send/recv and handle HD-related msg
PUBLIC void task_hd() {
    MESSAGE msg;
    init_hd();

    while (1) {
        send_recv(RECEIVE, ANY, &msg);

        int src = msg.source;
        switch(msg.type) {
            case DEV_OPEN:
                hd_open(msg.DEVICE);
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

    // initialize hd_info array
    for (int i = 0; i < (sizeof(hd_info) / sizeof(hd_info[0])); i++) {
        memset(&hd_info[i], 0, sizeof(hd_info[0]));
    }
    hd_info[0].open_cnt = 0;
}

/*
 * handles DEV_OPEN message, get the HD if the given device and read the partition table
 */
PRIVATE void hd_open(int device) {
    int drive = DRV_OF_DEV(device);   // must be 0 here, since only 1 HD
    assert(drive == 0);

    hd_identify(drive);

    if (hd_info[drive].open_cnt++ == 0) {
        partition(drive * (NR_PART_PER_DRIVE + 1), P_PRIMARY);
        print_hdinfo(&hd_info[drive]);
    }
}

// read one partition table of a given drive
// sect_nr: the sector in the drive locating the partition table
PRIVATE void get_part_table(int drive, int sect_nr, struct part_ent* entry) {
    struct hd_cmd cmd;
    cmd.features   = 0;
    cmd.count      = 1;
    cmd.lba_low    = sect_nr & 0xFF;
    cmd.lba_mid    = (sect_nr >> 8) & 0xFF;
    cmd.lba_high   = (sect_nr >> 16) & 0xFF;
    cmd.device     = MAKE_DEVICE_REG(1, drive, (sect_nr >> 24) & 0xF);
    cmd.command    = ATA_READ;
    hd_cmd_out(&cmd);
    interrupt_wait();

    port_read(REG_DATA, hdbuf, SECTOR_SIZE);
    memcpy(entry,
           hdbuf + PARTITION_TABLE_OFFSET,
           sizeof(struct part_ent) * NR_PART_PER_DRIVE);
}

// read all the partition table and fill the hd_info struct
PRIVATE void partition(int device, int style) {
    int i;
    int drive = DRV_OF_DEV(device);
    struct hd_info* hdi = &hd_info[drive];

    struct part_ent part_tbl[NR_SUB_PER_DRIVE];

    if (style == P_PRIMARY) {
        get_part_table(drive, drive, part_tbl);

        int nr_prim_parts = 0;
        for (i = 0; i < NR_PART_PER_DRIVE; i++) {
            if (part_tbl[i].sys_id == NO_PART) {
                continue;
            }
            nr_prim_parts++;
            int dev_nr = i + 1;  // 1-4
            hdi->primary[dev_nr].base = part_tbl[i].start_sect;
            hdi->primary[dev_nr].size = part_tbl[i].nr_sects;

            if (part_tbl[i].sys_id == EXT_PART) {   // continue to read the extended partition
                partition(device + dev_nr, P_EXTENDED);
            }
            assert(nr_prim_parts != 0);
        }
    } else if (style == P_EXTENDED) {
        int j = device % NR_PRIM_PER_DRIVE;  // 1-4
        int ext_start_sect = hdi->primary[j].base;
        int s = ext_start_sect;
        int nr_1st_sub = (j - 1) * NR_SUB_PER_PART;

        for (i = 0; i < NR_SUB_PER_PART; i++) {
            int dev_nr = nr_1st_sub + i;
            get_part_table(drive, s, part_tbl);
            hdi->logical[dev_nr].base = s + part_tbl[0].start_sect; // add the base offset, see Figure 9.7
            hdi->logical[dev_nr].size = part_tbl[0].nr_sects;

            /*
             * move s to the next logical partitions
             * each logical partitions contains one normal partition and one extended (05) parition,
             * the extended partition (part_tbl[1]) contains the address of the next logical partition
             * See Table 9.5 and 9.6
             */
            s = ext_start_sect + part_tbl[1].start_sect;

            if (part_tbl[1].sys_id == NO_PART) { // no more logical partitions in this extended
                break;
            }
        }
    } else {
        assert(0);
    }
}

// print out the disk info
PRIVATE void print_hdinfo(struct hd_info* hdi) {
    int i;
    for (i = 0; i < NR_PART_PER_DRIVE + 1; i++) {
        printl("%sPART_%d: base %d(0x%x), size %d(0x%x) (in sector)\n",
               i == 0 ? " " : "     ",   // 0 is for starting sector
               i,
               hdi->primary[i].base,
               hdi->primary[i].base,
               hdi->primary[i].size,
               hdi->primary[i].size);
    }
    for (i = 0; i < NR_SUB_PER_DRIVE; i++) {
        if (hdi->logical[i].size == 0) {
            continue;
        }
        printl("      "
               "%d: base %d(0x%x), size %d(0x%x) (in sector)\n",
               i,
               hdi->logical[i].base,
               hdi->logical[i].base,
               hdi->logical[i].size,
               hdi->logical[i].size);
    }
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

    // store the base/size info into the hd_info array
    u16* hdinfo = (u16*)hdbuf;
    hd_info[drive].primary[0].base = 0;
    // total number of User addressable Sectors
    hd_info[drive].primary[0].size = ((int)hdinfo[61] << 16) + hdinfo[60];
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