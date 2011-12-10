#include "cleaner.h"
#include "inode.h"
#include "segment.h"
#include "lfs.h"

// clean_cost returns the number of deadblocks present the segment
int clean_cost(int segment_num, char *ssbuf)
{
	int ino, blk, j, deadblock = 0; 
	struct inode *i;
	char *ibuf;
	ibuf = malloc(BLKSIZE);
	int segno = segment_num;

	read_from_log(segno, 0, ssbuf, BLKSIZE, 0);

	struct segsum *ss = (struct segsum *) ssbuf;
	for(j = 1; j < MAX_SEG_BLKS; j++)
	{
		ino = ss[j].inode_num;
                blk = ss[j].logical_blk;

		// represents its an inode
		if(blk == -1) 
		{
			//if segment number or block number in ifile dont match
			if((li->ino_map[ino].seg_num != segno) || (li->ino_map[ino].blk_num != j)) 
			{
				// mark as dead block
				ss[j].inode_num = -1;  
				deadblock++;
			}
		}
		else
		{
			read_from_log(li->ino_map[ino].seg_num, li->ino_map[ino].blk_num, ibuf, BLKSIZE, 0);
			i = (struct inode *) ibuf;
			
			// if block address in inode do not match
			if((i->direct[blk].seg_num != segno) || (i->direct[blk].blk_num != j)) 
			{
				// mark as a deadblock
				ss[j].inode_num = -1;  
				ss[j].logical_blk = -1;
				deadblock++;
			}
		}
	}
	free(ibuf);
	return deadblock;
}

void lfs_clean()
{

	char *new_buf = malloc(SEG_SIZE);
	char *ssbuf = malloc(BLKSIZE);
	int ino, blk, i, j, db;
	for(i =0; i < MAX_NUM_SEG; i++)
	{
       	    // if ith segment is not free
	    if(!li->seg_bitmap[i])
	    {
		db = clean_cost(i, ssbuf);
		if(db >= li->threshold)
		{
			// live blocks are to be copied into this segment	
			struct segsum *ss = (struct segsum *) li->cur_seg_buf; 
			read_from_log(i, 0, new_buf, SEG_SIZE, 0);

			// segment to be cleaned
			struct segsum *clean_ss = (struct segsum *) ssbuf ; 
			for(j = 1; j < MAX_SEG_BLKS; )
			{
				ino = clean_ss[j].inode_num;
				blk = clean_ss[j].logical_blk;

				// check if it is a deadblock
				if( ino != -1)  
				{
					// inode of the block not in memory
					if( li->ino_map[ino].seg_num != li->log_head) 
					{
						read_from_log(li->ino_map[ino].seg_num, li->ino_map[ino].blk_num, li->cur_seg_buf + li->cur_seg_blk *BLKSIZE, BLKSIZE, 0);

						// update the inode map
						li->ino_map[ino].seg_num = li->log_head; 
						li->ino_map[ino].blk_num = li->cur_seg_blk;
					
						// update the segment summary
						ss[li->cur_seg_blk].inode_num = ino;  

						// mark as inode 
						ss[li->cur_seg_blk].logical_blk = -1; 
					
						copy_segmentdata_to_log(li->fd,li->cur_seg_buf, SEG_SIZE,li->log_head * SEG_SIZE + BLKSIZE);
						li->cur_seg_blk++;
					}
					// check if the current block in clean segment is an inode	
					if( blk == -1) 
					{
						j++;
						// as inode is copied, continue with the process
						continue;  
					}
					struct inode *id = (struct inode *) (li->cur_seg_buf + li->ino_map[ino].blk_num * BLKSIZE); 

					// copy the block from new_buf into current segment.
					memmove(li->cur_seg_buf + li->cur_seg_blk *BLKSIZE, new_buf + j*BLKSIZE, BLKSIZE); 
					// update the inode
					id->direct[blk].seg_num = li->log_head; 
					id->direct[blk].blk_num = li->cur_seg_blk;
					

					// update segment summary as 1 block used
					ss[li->cur_seg_blk].inode_num = ino; 
					ss[li->cur_seg_blk].logical_blk = blk; 
					copy_segmentdata_to_log(li->fd, li->cur_seg_buf, SEG_SIZE,li->log_head * SEG_SIZE + BLKSIZE);
					li->cur_seg_blk++;
				}
				// consider next block in clean segment.
				j++;
			}	
			// Mark ith segment as free
			li->seg_bitmap[i] = 1;
		    }
		}
	}
	return;
}

