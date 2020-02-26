#ifndef _ORANGES_HD_H_
#define _ORANGES_HD_H_

/*
 * partition entry struct (16 bytes), see Table 9.3
 */
struct part_ent {
    u8 boot_ind;      // only 80h or 00h
    u8 start_head;
    u8 start_sector;
    u8 start_cyl;
    u8 sys_id;
    u8 end_head;
    u8 end_sector;
    u8 end_cyl;
    u32 start_sect;   // counted from 0, in LBA, see https://en.wikipedia.org/wiki/Logical_block_addressing#CHS_conversion
    u32 nr_sects;
} PARTIRION_ENTRY;

/* I/O ports for HD controllers, see Table 9.1. */
/* Currently only support master device, 1st col of Table 9.1 */
// command block registers
#define REG_DATA      0x1F0
#define REG_FEATURES  0x1F1
#define REG_ERROR     REG_FEATURES
#define REG_NSECTOR   0x1F2
#define REG_LBA_LOW   0x1F3
#define REG_LBA_MID   0x1F4
#define REG_LBA_HIGH  0x1F5
#define REG_DEVICE    0x1F6
#define REG_STATUS    0x1F7

/* status register bit def, see Figure 9.3 */
#define	STATUS_BSY	0x80
#define	STATUS_DRDY	0x40
#define	STATUS_DFSE	0x20
#define	STATUS_DSC	0x10
#define	STATUS_DRQ	0x08
#define	STATUS_CORR	0x04
#define	STATUS_IDX	0x02
#define	STATUS_ERR	0x01

#define REG_CMD		REG_STATUS

// control block registers
#define REG_DEV_CTRL   0x3F6
#define REG_ALT_STATUs REG_DEV_CTRL

#define REG_DRV_ADDR   0x3F7

#define MAX_IO_BYTES   256      // max sectors IO can handle

// command register values
struct hd_cmd {
    u8 features;
    u8 count;
    u8 lba_low;
    u8 lba_mid;
    u8 lba_high;
    u8 device;
    u8 command;
};

// info for one partition
struct part_info {
    u32 base;      // # of start sector
    u32 size;      // # of sectors in the partition
};

// one entry per drive, recording partition infos of primary and logical
struct hd_info {
    int                  open_cnt;
    struct part_info     primary[NR_PRIM_PER_DRIVE]; // primary[0] records base and size of the starting sector of the HD
    struct part_info     logical[NR_SUB_PER_DRIVE];
};

/* Constants */
#define HD_TIMEOUT              10000   // in ms
#define PARTITION_TABLE_OFFSET  0x1BE
#define ATA_IDENTIFY            0xEC
#define ATA_READ                0x20
#define ATA_WRITE               0x30

#define MAKE_DEVICE_REG(lba, drv, lba_highest) ((lba << 6) | (drv << 4) | \
                                                (lba_highest & 0xF) | 0xA0)


#endif /*_ORANGES_HD_H */