#ifndef _ORANGES_HD_H_
#define _ORANGES_HD_H_

struct part_ent {
    u8 boot_ind;
    u8 start_head;
    u8 start_sector;
    u8 start_cyl;
    u8 sys_id;
    u8 end_head;
    u8 end_sector;
    u8 end_cyl;
    u32 start_sect;
    u32 nr_sects;
} PARTIRION_ENTRY;

/* I/O ports for HD controllers, see Table 9.1. */
/* Currently only support master device, 1st col of Table 9.1 */
// command block registers
#define REG_DATA      0x1F0
#define REG_FEATURES  0x1F1
#define REG_ERROR     REG_FEATURES
#define REF_NSECTOR   0x1F2
#define REG_LBA_LOW   0x1F3
#define REG_LBA_MID   0x1F4
#define REF_LBA_HIGH  0x1F5
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

/* Constants */
#define HD_TIMEOUT              10000   // in ms
#define PARTITION_TABLE_OFFSET  0x1BE
#define ATA_IDENTITY            0xEC
#define ATA_READ                0x20
#define ATA_WRITE               0x30

#define MAKE_DEVICE_REG(lba, drv, lba_highest) ((lba << 6) | (drv << 4) | \
                                                (lba_highest & 0xF) | 0xA0)


#endif /*_ORANGES_HD_H */