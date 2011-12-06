struct superblock {
	int n_inodes;		// number of inodes in the system
	int n_blocks;	 	// number of blocks in the system
	int n_free_blocks;	// number of free blocks
	int cur_seg;		// current segment 
	int ifile_addr;		// address of inode map block
	int seg_offset;		// offset inside the segment
	int seg_size;		// size of each segment
	int block_size;		// size of each block
};

void superblock_init();
void superblock_update();

