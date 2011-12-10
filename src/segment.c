#include<unistd.h>
#include "segment.h"
#include"lfs.h"

// get_next_free_segment returns the number of next free segment
// to which the global log_head is made to point to
int get_next_free_segment()
{
        int i,j;
	j = li->log_head + 1;
        for(i = 0; i < MAX_NUM_SEG ; i++,j++) 
        {
		if( j == MAX_NUM_SEG )
			j = 0;
                if(li->seg_bitmap[j] == 1)
                {
                        return j;
                }
        }
        return -1;
}

int num_of_free_segments()
{
	int i, count = 0;
	for(i = 0; i < MAX_NUM_SEG; i++)
	{
		if(li->seg_bitmap[i] == 1)
			count++;
	}
	return count;
}

void copy_segmentdata_to_log(int fd, char * buf, size_t count, off_t offset )
{
        size_t ret;
        struct segsum *ss = (struct segsum*)(li->cur_seg_buf);
        // if this is the last block in segment, write it into disc. 
        if( li->cur_seg_blk == MAX_SEG_BLKS -1)
        {
                ret = pwrite(fd, buf, count, offset);
                assert(ret == count);

                // update the bitmap indicating that the segment is not free
                li->seg_bitmap[li->log_head] = 0;

                li->log_head = get_next_free_segment();
		
		if(num_of_free_segments() <= 5)
		{
			li->cur_seg_blk = 1;
			lfs_clean();
		}

                li->cur_seg_blk = 0;

                // reset the memory buffer to zer0
                memset((void*)li->cur_seg_buf,0,SEG_SIZE);

                // resetting values for the segment summary entry for new segment
                ss[0].inode_num = -1;
                ss[0].logical_blk = -1;
        }
        return;
}


void read_from_log(int seg_num, int block_num, char *buf, int size, int blk_offset)
{
        int offset;
        offset = seg_num * SEG_SIZE + block_num * BLKSIZE + BLKSIZE + blk_offset;
        pread(li->fd, buf, size, offset);
        return;
}

