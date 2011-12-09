#include "segment.h"
#include"lfs.h"

// get_next_free_segment returns the number of next free segment
// to which the global log_head is made to point to
int get_next_free_segment()
{
        int i;

        for(i = 0; i < MAX_NUM_SEG ; i++) 
        {
                if(li->seg_bitmap[i] == 1)
                {
                        return i;
                }
        }
        return -1;
}

int copy_buffer_to_segment()
{
}

int write_segment_to_file(){
}
