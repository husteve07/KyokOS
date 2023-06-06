#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int KYOKOS_DISK_TYPE;

// represents a real physical hard disk
#define KYOKOS_DISK_TYPE_REAL 0

struct disk
{
    KYOKOS_DISK_TYPE type;
    int sector_size;

    //id of disk
    int id;

    struct filesystem* filesystem;

    //private data of filesystem
    void* fs_private;
};


int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);
struct disk* disk_get(int index);
void disk_search_and_init();

#endif