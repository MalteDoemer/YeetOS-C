#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "errno.h"

#include "kernel/kernel.h"

#if !defined(__x86__) && !defined(__x64__)
#error "ata only supported on x86 or x64"
#endif

#ifdef __x86__
#include "arch/x86/asm.h"
#endif

#define ATA_DATA 0
#define ATA_ERROR 1
#define ATA_SECTOR_COUNT 2
#define ATA_LBA_LOW 3
#define ATA_LBA_MID 4
#define ATA_LBA_HIGH 5
#define ATA_DEVICE 6
#define ATA_COMMAND 7
#define ATA_STATUS 7
#define ATA_CTRL 0x206

#define ATA_PRIMARY 0x1F0
#define ATA_SECONDARY 0x170

#define ATA_DEVICE_MASTER 0xA0
#define ATA_DEVICE_SLAVE 0xB0

#define MAX_ATA_DRV 20

#define HDA 0
#define HDB 1
#define HDC 2
#define HDD 3

typedef struct ata_drv_t {
    uint16_t port;      /* 0x1F0 for primary, 0x170 for secondary */
    uint8_t device;     /* Master or slave */
    uint32_t bps;       /* Bytes per sector -> 512 */
    uint64_t start_lba; /* Start LBA, used for partitioning */
    uint64_t end_lba;   /* End LBA, used for partitioning */
} ata_drv_t;

ata_drv_t ata_drvs[MAX_ATA_DRV];

uint8_t ata_poll_status(ata_drv_t* drv)
{
    uint8_t status = inb(drv->port + ATA_STATUS);

    while ((status & 0x80) && !(status & 0x01)) {
        status = inb(drv->port + ATA_STATUS);
    }

    return status;
}

int ata_flush(ata_drv_t* drv)
{
    outb(drv->port + ATA_DEVICE, drv->device + 0x40);
    outb(drv->port + ATA_COMMAND, 0xE7);

    uint8_t status = ata_poll_status(drv);

    if (status == 0 || status & 0x01)
        return ERROR;

    return SUCCESS;
}

int ata_identify(ata_drv_t* drv)
{
    outb(drv->port + ATA_DEVICE, drv->device);

    outb(drv->port + ATA_SECTOR_COUNT, 0);
    outb(drv->port + ATA_LBA_LOW, 0);
    outb(drv->port + ATA_LBA_MID, 0);
    outb(drv->port + ATA_LBA_HIGH, 0);
    outb(drv->port + ATA_COMMAND, 0xEC);

    uint8_t status = ata_poll_status(drv);

    if (status == 0 || status & 0x01)
        return ERROR;

    for (size_t i = 0; i < 256; i++) {
        uint16_t data = inw(drv->port + ATA_DATA);
        (void)data;
    }

    return SUCCESS;
}

int ata_write28(ata_drv_t* drv, uint64_t lba, uint8_t* buffer)
{
    if (lba < drv->start_lba || lba > drv->start_lba) {
        return ERROR;
    }

    outb(drv->port + ATA_DEVICE, (drv->device + 0x40) | ((lba >> 24) & 0xF));
    outb(drv->port + ATA_ERROR, 0);
    outb(drv->port + ATA_SECTOR_COUNT, 1);

    outb(drv->port + ATA_LBA_LOW, (lba >> 0) & 0xFF);
    outb(drv->port + ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(drv->port + ATA_LBA_HIGH, (lba >> 16) & 0xFF);

    outb(drv->port + ATA_COMMAND, 0x30);

    uint8_t status = ata_poll_status(drv);
    if (status == 0 || status & 0x01)
        return ERROR;

    uint16_t* buf = (uint16_t*)buffer;
    for (size_t i = 0; i < drv->bps / 2; i++) {
        outw(drv->port + ATA_DATA, buf[i]);
    }

    ata_flush(drv);

    return SUCCESS;
}

int ata_read28(ata_drv_t* drv, uint64_t lba, uint8_t* buffer)
{
    if (lba < drv->start_lba || lba > drv->start_lba) {
        return ERROR;
    }

    outb(drv->port + ATA_DEVICE, (drv->device + 0x40) | ((lba >> 24) & 0xF));
    outb(drv->port + ATA_ERROR, 0);
    outb(drv->port + ATA_SECTOR_COUNT, 1);

    outb(drv->port + ATA_LBA_LOW, (lba >> 0) & 0xFF);
    outb(drv->port + ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(drv->port + ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(drv->port + ATA_COMMAND, 0x20);

    uint8_t status = ata_poll_status(drv);
    if (status == 0 || status & 0x01)
        return ERROR;

    uint16_t* buf = (uint16_t*)buffer;
    for (size_t i = 0; i < drv->bps / 2; i++) {
        buf[i] = inw(drv->port + ATA_DATA);
    }

    return SUCCESS;
}

CTOR int init_ata()
{
    return SUCCESS;
}