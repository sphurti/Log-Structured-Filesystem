#ifndef INODE_H
#define INODE_H

#include"segment.h"
#include"lfs.h"

struct direct_blk {
	uint16_t seg_num;
	uint16_t blk_num;
};
struct inode {
	uint32_t ino;	// inode number
	uint32_t mode;	// 
	uint32_t size;
	uint32_t atime;
	uint32_t mtime;
	uint32_t dtime;
	uint32_t link_cnt;
	uint32_t block_cnt;
	struct direct_blk direct[MAX_BLKS_FOR_FILE];
};

void alloc_inode();
struct inode* read_inode();
int is_inode_free(struct inode *);

#endif
