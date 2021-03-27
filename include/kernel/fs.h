#ifndef FS_H
#define FS_H


#include "stdint.h"
#include "stddef.h"

struct fs_node_t;
struct fs_desc_t;

typedef size_t (*read_func_t)(struct fs_node_t* node, struct fs_desc_t* desc, char* buffer, size_t size);
typedef size_t (*write_func_t)(struct fs_node_t* node, struct fs_desc_t* desc, char* buffer, size_t size);



#endif // #ifndef FS_H
