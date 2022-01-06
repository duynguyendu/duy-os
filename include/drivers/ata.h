#ifndef __ATA_H__
#define __ATA_H__

#include <types.h>

/* ATA 8-bit status bit mask */
#define ATA_STT_BUSY 0x80
#define ATA_STT_DRIVE_READY 0x40
#define ATA_STT_WRITE_FAULT 0x20
#define ATA_STT_READ_COMP 0x10
#define ATA_STT_REQUEST_READY 0x08
#define ATA_STT_CORRECTED_DATA 0x04
#define ATA_STT_INDEX 0x02
#define ATA_STT_ERROR 0x01

/* ATA 8-bit error bit mask */
#define ATA_ERR_BAD_BLOCK 0x80
#define ATA_ERR_UNCORRECTED_DATA 0x40
#define ATA_ERR_MEDIA_CHANGE 0x20
#define ATA_ERR_ID_MARK_NOT_FOUND 0x10
#define ATA_ERR_MEDIA_CHANGE_REQUETS 0x08
#define ATA_ERR_COMMAND_ABORT 0x04
#define ATA_ERR_TRACK0_NOT_FOUND 0x02
#define ATA_ERR_NO_ADDRESS_MASK 0x01

/* Command is issued when write to command/status port */
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_READ_PIO_EXT 0x24
#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EXT 0x25
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_WRITE_PIO_EXT 0x34
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EXT 0x35
#define ATA_CMD_CACHE_FLUSH 0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA
#define ATA_CMD_PACKET 0xA0
#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY 0xEC

/* ATA registers, the value below is the offset from the base address of each
 * device */
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_HDDEVSEL 0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS 0x07
#define ATA_REG_SECCOUNT1 0x08
#define ATA_REG_LBA3 0x09
#define ATA_REG_LBA4 0x0A
#define ATA_REG_LBA5 0x0B
#define ATA_REG_CONTROL 0x0C
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_DEVADDRESS 0x0D

/* ATA identify packet buffer */
#define ATA_IDENT_DEVICETYPE 0
#define ATA_IDENT_CYLINDERS 2
#define ATA_IDENT_HEADS 6
#define ATA_IDENT_SECTORS 12
#define ATA_IDENT_SERIAL 20
#define ATA_IDENT_MODEL 54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID 106
#define ATA_IDENT_MAX_LBA 120
#define ATA_IDENT_COMMANDSETS 164
#define ATA_IDENT_MAX_LBA_EXT 200

#define IDE_ATA 0x00
#define IDE_ATAPI 0x01

#define ATA_MASTER 0x0
#define ATA_SLAVE 0x1

struct channel {
    uint16_t base;
    uint16_t ctrl;
    uint16_t master_ide;
};
typedef struct channel channel_t;

struct ata_dev {
    uint8_t type;
    uint8_t channel;
    uint8_t drive;
    uint16_t config;
    uint16_t capabilities;
    uint32_t command_set;
    uint32_t size;
    uint8_t model[41];
};
typedef struct ata_dev ata_dev_t;

void ata_init(uint8_t bus, uint8_t device, uint8_t func);

#endif // !__ATA_H__
