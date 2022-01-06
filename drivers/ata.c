#include <asm/interrupt.h>
#include <asm/ports.h>
#include <drivers/ata.h>
#include <drivers/pci.h>
#include <idt.h>
#include <memory.h>
#include <stdio.h>
#include <types.h>

#define ATA_PRIMARY 0
#define ATA_SECONDARY 1
#define IDENTITY_LENGTH 512
#define REG(i, R) (channels[i].base + R)

channel_t channels[2] = {{.base = 0x1F0, .ctrl = 0x3F6, .master_ide = 0},
                         {.base = 0x170, .ctrl = 0x376, .master_ide = 8}};
ata_dev_t *devices[4];

static void ata_io_wait(int i);
static void inline ata_status_wait(int i);
static void inline ata_wait_bsy(int i);
static void inline ata_wait_drq(int i);
static void inline software_reset(int i);
static void read_buffer(int i, uint16_t *buf, int length);
static void ata_read(int i, int j, uint32_t addr, uint32_t lba,
                     uint8_t sector_count);
static void ata_write(int i, int j, uint32_t addr, uint32_t lba,
                      uint8_t sector_count);

uint8_t ident[IDENTITY_LENGTH];
uint8_t buffer[5120];

void ata_init(uint8_t bus, uint8_t device, uint8_t func) {
    uint8_t count = 0;
    uint8_t total = 0;
    port_byte_out(channels[0].ctrl, 0x02); // Bit 2 indicate disable interrupt
    port_byte_out(channels[1].ctrl, 0x02);
    for (int i = 0; i < 2; i++) {
        /* Reset both master and slave device */
        software_reset(i);

        for (int j = 0; j < 2; j++) {
            /* Select drive (j = 0 for master and 1 for slave) */
            port_byte_out(REG(i, ATA_REG_HDDEVSEL), 0xA0 | (j << 4));
            ata_io_wait(i);

            /* Environment set before sending identify command */
            port_byte_out(REG(i, ATA_REG_SECCOUNT0), 0);
            port_byte_out(REG(i, ATA_REG_LBA0), 0);
            port_byte_out(REG(i, ATA_REG_LBA1), 0);
            port_byte_out(REG(i, ATA_REG_LBA2), 0);

            /* Sending identify command */
            port_byte_out(REG(i, ATA_REG_COMMAND), ATA_CMD_IDENTIFY);
            ata_io_wait(i);

            /* Check the status */
            uint8_t status = port_byte_in(REG(i, ATA_REG_STATUS));
            /* status = 0, no device */
            if (!status)
                continue;
            ata_status_wait(i);

            devices[count] = (ata_dev_t *)kmalloc(sizeof(ata_dev_t));
            /* Read the identification */
            read_buffer(i, (uint16_t *)ident, 256);
            devices[count]->config =
                *((uint16_t *)(ident + ATA_IDENT_DEVICETYPE));
            devices[count]->capabilities =
                *((uint16_t *)(ident + ATA_IDENT_CAPABILITIES));
            devices[count]->command_set =
                *((uint32_t *)(ident + ATA_IDENT_COMMANDSETS));
            devices[count]->size = *((uint32_t *)(ident + ATA_IDENT_MAX_LBA));
            for (int k = 0; k < 20; k++) {
                devices[count]->model[2 * k + 1] =
                    ident[ATA_IDENT_MODEL + 2 * k];
                devices[count]->model[2 * k] =
                    ident[ATA_IDENT_MODEL + 2 * k + 1];
            }
            devices[count]->model[40] = '\0';

            /* Check the type of ATA device */
            uint8_t lba_lo, lba_hi;
            lba_lo = port_byte_in(REG(i, ATA_REG_LBA1));
            lba_hi = port_byte_in(REG(i, ATA_REG_LBA2));
            /* Default type */
            devices[count]->type = IDE_ATA;
            if (lba_lo == 0x14 && lba_hi == 0xEB) {
                devices[count]->type = IDE_ATAPI;
            }

            devices[count]->channel = i;
            devices[count]->drive = j;

            kprintf("Model: %s Size: %dKb\n", devices[count]->model, devices[count]->size / 1024);
            total++;
        }
    }
    kprintf("[ATA] Init %d Disk\n", total);
}

static void ata_read(int i, int j, uint32_t addr, uint32_t lba,
                     uint8_t sector_count) {
    ata_wait_bsy(i);
    port_byte_out(REG(i, ATA_REG_HDDEVSEL),
                  0xE0 | (j << 4) | ((lba >> 24) & 0xF));
    ata_io_wait(i);
    port_byte_out(REG(i, ATA_REG_SECCOUNT0), sector_count);
    port_byte_out(REG(i, ATA_REG_LBA0), (uint8_t)lba);
    port_byte_out(REG(i, ATA_REG_LBA1), (uint8_t)(lba >> 8));
    port_byte_out(REG(i, ATA_REG_LBA2), (uint8_t)(lba >> 16));
    port_byte_out(REG(i, ATA_REG_COMMAND), ATA_CMD_READ_PIO);

    uint16_t *temp = (uint16_t *)addr;
    for (int j = 0; j < sector_count; j++) {
        ata_status_wait(i);
        for (int h = 0; h < 256; h++) {
            temp[h] = port_word_in(REG(i, ATA_REG_DATA));
        }
        temp += 256;
    }
}

static void ata_write(int i, int j, uint32_t addr, uint32_t lba,
                      uint8_t sector_count) {
    ata_wait_bsy(i);
    port_byte_out(REG(i, ATA_REG_HDDEVSEL),
                  0xE0 | (j << 4) | ((lba >> 24) & 0xF));
    port_byte_out(REG(i, ATA_REG_SECCOUNT0), sector_count);
    port_byte_out(REG(i, ATA_REG_LBA0), (uint8_t)lba);
    port_byte_out(REG(i, ATA_REG_LBA1), (uint8_t)(lba >> 8));
    port_byte_out(REG(i, ATA_REG_LBA2), (uint8_t)(lba >> 16));
    port_byte_out(REG(i, ATA_REG_COMMAND), ATA_CMD_WRITE_PIO);

    uint16_t *temp = (uint16_t *) addr;
    for (int j = 0; j < sector_count; j++) {
        ata_status_wait(i);
        for (int h = 0; h < 256; h++) {
            port_word_out(REG(i, ATA_REG_DATA), temp[h]);
        }
    }
}
static void ata_io_wait(int i) {
    /* Wait 400ns for disk to complete gathering data, each read cost 400ns */
    port_byte_in(REG(i, ATA_REG_STATUS));
    port_byte_in(REG(i, ATA_REG_STATUS));
    port_byte_in(REG(i, ATA_REG_STATUS));
    port_byte_in(REG(i, ATA_REG_STATUS));
}

static void inline ata_wait_bsy(int i) {
    /* Wait until the drive is not ready */
    while (port_byte_in(REG(i, ATA_REG_STATUS)) & ATA_STT_BUSY)
        ;
}
static void inline ata_wait_drq(int i) {
    /* Wait until the processing is finished */
    while (!(port_byte_in(REG(i, ATA_REG_STATUS)) & ATA_STT_DRIVE_READY))
        ;
}

static void inline ata_status_wait(int i) {
    /* Wait until the drive is not busy and has finished processing */
    ata_wait_bsy(i);
    ata_wait_drq(i);
}

static void inline software_reset(int i) {
    /* Tell the drive to reset itself by setting bit 2 in control register*/
    port_byte_out(channels[i].ctrl, 0x04);
    /* Wait for the drive to reset */
    ata_io_wait(i);
    /* Unset bit 2 to stop resetting device */
    port_byte_out(channels[i].ctrl, 0x00);
}

static void read_buffer(int i, uint16_t *buf, int length) {
    for (int j = 0; j < length; j++) {
        buf[j] = port_word_in(REG(i, ATA_REG_DATA));
    }
}
