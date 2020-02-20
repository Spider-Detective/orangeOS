#ifndef _ORANGES_FS_H_
#define _ORANGES_FS_H_

struct dev_drv_map {
    int driver_nr;
};

#define MAGIC_V1      0x111

// see Page 357. super block takes up 1 sector (512 bytes)
struct super_block {
    u32    magic;
    u32    nr_inodes;
    u32    nr_sects;
    u32    nr_imap_sects;
    u32    nr_smap_sects;
    u32    n_1st_sect;

    u32    nr_inode_sects;
    u32    root_inode;
    u32    inode_size;
    u32    inode_isize_off;
    u32    inode_start_off;
    u32    dir_ent_size;
    u32    dir_ent_inode_off;
    u32    dir_ent_fname_off;

    // this stores the home device number after reading super block into memory
    int    sb_dev;
};

#define SUPER_BLOCK_SIZE          56

// stores 1 file's info in an inode
struct inode {
    u32    i_mode;             // access mode
    u32    i_size;             // file size
    u32    i_start_sect;       // first sector of data
    u32    i_nr_sects;
    u8     _unused[16];        // for alignment

    // write into these values after reading into memory
    int    i_dev;
    int    i_cnt;
    int    i_num;
};

#define INODE_SIZE              32
#define MAX_FILENAME_LEN        12

// store the node and name for one directory
struct dir_entry {
    int    inode_nr;
    char   name[MAX_FILENAME_LEN]
};

#define DIR_ENTRY_SIZE  sizeof(struct dir_entry)

// file descriptor, in file descriptor table (array), see Figure 9.12
struct file_desc {
    int            fd_mode;   // read or write
    int            fd_pos;    // current pos for R/W
    struct inode*  fd_inode;  // pointer to inode in i_node_table
};

// a wrapper for rw_sector to reduce the argument number
#define RD_SECT(dev, sect_nr) rw_sector(DEV_READ, \
                                        dev,      \
                                        (sect_nr)* SECTOR_SIZE,   \
                                        SECTOR_SIZE,           \
                                        TASK_FS,     \
                                        fsbuf);

#define WR_SECT(dev, sect_nr) rw_sector(DEV_WRITE, \
                                        dev,      \
                                        (sect_nr)* SECTOR_SIZE,   \
                                        SECTOR_SIZE,           \
                                        TASK_FS,     \
                                        fsbuf);

#endif /* _ORANGES_FS_H */