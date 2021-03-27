#ifndef FS_H
#define FS_H

#define MAX_NAME 64

#define MAX_CHRDEV 32
#define MAX_BLKDEV 32

#define FS_FILE 1
#define FS_DIR 2
#define FS_CHRDEV 3
#define FS_BLKDEV 4

typedef long long fpos_t;
typedef int mode_t;

struct fs_node_t;
struct fs_desc_t;
struct fs_ops_t;

typedef struct dev_t {
    uint16_t major;
    uint16_t minor;
} dev_t;

typedef int (*seek_func_t)(struct fs_node_t* node, struct fs_desc_t* desc, fpos_t offset, mode_t mode);
typedef int (*read_func_t)(struct fs_node_t* node, struct fs_desc_t* desc, char* buffer, size_t size);
typedef int (*write_func_t)(struct fs_node_t* node, struct fs_desc_t* desc, char* buffer, size_t size);
typedef int (*open_func_t)(struct fs_node_t* node, struct fs_desc_t* desc);
typedef int (*close_func_t)(struct fs_node_t* node, struct fs_desc_t* desc);
//typedef int (*readdir_func_t) (struct fs_node_t *, struct fs_desc_t *, struct dirent_t *, int);

typedef struct fs_ops_t {
    seek_func_t seek;
    read_func_t read;
    write_func_t write;
    open_func_t open;
    close_func_t close;
} fs_ops_t;

typedef struct fs_desc_t {
    mode_t mode;
    fpos_t offset;
    size_t count;
    struct fs_node_t* node;
    void* private;
} fs_desc_t;

typedef struct fs_node_t {
    char name[MAX_NAME];
    uint32_t type;
    uint32_t flags;
    fs_ops_t ops;
    dev_t dev;
    struct fs_node_t* next;
    struct fs_node_t* down;
} fs_node_t;


extern int register_chrdev(uint16_t major, const char* name, fs_ops_t* ops);
extern int register_blkdev(uint16_t major, const char* name, fs_ops_t* ops);

extern int unregister_chrdev(uint16_t major);
extern int unregister_blkdev(uint16_t major);

#endif // #ifndef FS_H
