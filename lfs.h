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
#define SEG_SIZE	64*BLKSIZE	// size of segment in bytes
#define MAX_SEG_BLKS	3		// maximum number of blocks per segment
#define MAX_NUM_SEG     32		// maximum number of segements
#define MAX_BLKS_FOR_FILE	1000	// Maximum number of blocks a file  can occupy
#define MAX_INODES	1024

#define MIN(a,b)	(a < b ? a : b)
struct inode_map_entry {
	uint16_t seg_num;
	uint16_t blk_num;
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
	uint16_t log_head;
	// the rbtree for inode map entrees
	struct inode_map_entry ino_map[MAX_INODES];
	// segment bitmap
	uint32_t *seg_bitmap;

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
void read_from_disc(int seg_num, int block_num, char *buf, int size, int blk_offset);
void copy_segmentdata_to_disk(int fd, char * buf, size_t count, off_t offset);
char* get_filename(char *path);


// other functions


#endif
