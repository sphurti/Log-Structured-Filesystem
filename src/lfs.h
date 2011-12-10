#ifndef LFS_H
#define LFS_H

#include<fuse.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<unistd.h>
#include "segment.h"
#include"uthash.h"
#include <assert.h>

#define MAXNAMELEN	128
#define BLKSIZE		4096
#define SEG_SIZE	6*BLKSIZE	// size of segment in bytes
#define MAX_SEG_BLKS	6		// maximum number of blocks per segment
#define MAX_NUM_SEG     8		// maximum number of segements
#define MAX_BLKS_FOR_FILE	500	// Maximum number of blocks a file  can occupy
#define MAX_INODES	1024

// the minimum number of deadblocks in the segment that should be present for cleaning
#define THRESHOLD	(MAX_SEG_BLKS / 2)	
#define MIN(a,b)	(a < b ? a : b)


struct inode_map_entry {
	int32_t seg_num;
	int32_t blk_num;
};



struct file_inode_hash {
        char f_name[MAXNAMELEN];
        uint32_t inode_num;
	uint32_t f_size;
        UT_hash_handle hh;
};

struct lfs_global_info {
	// hash table mapping filename to file_inode_hash structure
	struct file_inode_hash *fih;
	// current segment descriptor
	char  *cur_seg_buf;
	// current block of the current segment
	uint32_t cur_seg_blk;
	// number of inodes created
	uint64_t n_inode;
	// segment number of next free segment
	int32_t log_head;
	// the rbtree for inode map entrees
	struct inode_map_entry ino_map[MAX_INODES];
	// segment bitmap
	uint16_t seg_bitmap[MAX_NUM_SEG];
	// threshold that determines when the segment needs to be cleaned
	uint32_t threshold;

	// the file descriptor of the file representing the disk log
	int fd;	 
};

struct lfs_global_info *li;

void lfs_init();


// file operations
int lfs_open(const char *path, struct fuse_file_info *fi);
int lfs_create(const char *path, mode_t mode,struct fuse_file_info *fi);
int lfs_read(const char *path, char *buf, size_t count, off_t offset,struct fuse_file_info *fi);
int lfs_write(const char *path, char *buf, int count, int offset, struct fuse_file_info *fi);

// generic functions
char* get_filename(const char *path);




#endif
