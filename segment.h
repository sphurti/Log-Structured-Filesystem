#ifndef SEGMENT_H
#define SEGMENT_H

#include"lfs.h"


struct segsum {
	int32_t inode_num;	// inode number of the file
	int32_t logical_blk;	// logical block number of the file
};

int get_next_free_segment();

int copy_buffer_to_segment();
int write_segment_to_file();

#endif
