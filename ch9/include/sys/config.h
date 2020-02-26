/*
 * Definitions for hard disk configuration
 */

#define MINOR_BOOT           MINOR_hd2a

// consts for disk log
#define ENABLE_DISK_LOG
#define SET_LOG_SECT_SMAP_AT_STARTUP
#define MEMSET_LOG_SECTS
#define NR_SECTS_FOR_LOG       NR_DEFAULT_FILE_SECTS