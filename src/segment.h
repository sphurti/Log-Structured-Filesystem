#ifndef SEGMENT_H
#define SEGMENT_H

#include"lfs.h"
#include"cleaner.h"

struct segsum {
	int32_t inode_num;	// inode number of the file
	int32_t logical_blk;	// logical block number of the file
};

int get_next_free_segment();
int num_of_free_segments();

void read_from_log(int seg_num, int block_num, char *buf, int size, int blk_offset);
void copy_segmentdata_to_log(int fd, char * buf, size_t count, off_t offset);

#endif
