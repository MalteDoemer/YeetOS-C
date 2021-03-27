#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"

#include "kernel/kernel.h"
#include "kernel/kheap.h"
#include "kernel/panic.h"
#include "kernel/fs.h"

typedef struct device_struct_t {
    const char* name;
    fs_ops_t* ops;
} device_struct_t;

static device_struct_t chrdevs[MAX_CHRDEV] = { { NULL, NULL } };
static device_struct_t blkdevs[MAX_BLKDEV] = { { NULL, NULL } };

int register_chrdev(uint16_t major, const char* name, fs_ops_t* ops)
{
    if (major == 0) {
        for (size_t i = 1; i < MAX_CHRDEV; i++) {
            if (chrdevs[i].ops == NULL) {
                chrdevs[i].name = name;
                chrdevs[i].ops = ops;
                return (int)major;
            }
        }
        errno = EBUSY;
        return ERROR;
    }

    if (major >= MAX_CHRDEV) {
        errno = EINVAL;
        return ERROR;
    }

    if (chrdevs[major].ops && chrdevs[major].ops != ops) {
        errno = EBUSY;
        return ERROR;
    }

    chrdevs[major].name = name;
    chrdevs[major].ops = ops;
    return SUCCESS;
}

int register_blkdev(uint16_t major, const char* name, fs_ops_t* ops)
{
    if (major == 0) {
        for (size_t i = 1; i < MAX_BLKDEV; i++) {
            if (blkdevs[i].ops == NULL) {
                blkdevs[i].name = name;
                blkdevs[i].ops = ops;
                return (int)major;
            }
        }
        errno = EBUSY;
        return ERROR;
    }

    if (major >= MAX_BLKDEV) {
        errno = EINVAL;
        return ERROR;
    }

    if (blkdevs[major].ops && blkdevs[major].ops != ops) {
        errno = EBUSY;
        return ERROR;
    }

    blkdevs[major].name = name;
    blkdevs[major].ops = ops;
    return SUCCESS;
}

int unregister_chrdev(uint16_t major)
{
    if (major >= MAX_CHRDEV) {
        errno = EINVAL;
        return ERROR;
    }

    if (!chrdevs[major].ops) {
        errno = EINVAL;
        return ERROR;
    }

    chrdevs[major].name = NULL;
    chrdevs[major].ops = NULL;
    return SUCCESS;
}

int unregister_blkdev(uint16_t major)
{
    if (major >= MAX_BLKDEV) {
        errno = EINVAL;
        return ERROR;
    }

    if (!blkdevs[major].ops) {
        errno = EINVAL;
        return ERROR;
    }

    blkdevs[major].name = NULL;
    blkdevs[major].ops = NULL;
    return SUCCESS;
}

int chrdev_open(fs_node_t* node, fs_desc_t* desc)
{
    uint16_t major = node->dev.major;

    if (major >= MAX_CHRDEV || !chrdevs[major].ops)
    {
        errno = ENODEV;
        return ERROR;
    }
}