#include<stdlib.h>
#include<stdio.h>
#include"lfs.h"
#include"uthash.h"
#include"segment.h"
#include"inode.h"
#include<unistd.h>
#include<fuse.h>

// -DFUSE_USE_VERSION=22
// initialise all the global variables
void lfs_init()
{
	li = (struct lfs_global_info*)malloc(sizeof(struct lfs_global_info));
	//initialise global variables
	li->cur_seg_buf    = malloc(SEG_SIZE);
	li->fih 	   = NULL;
	li->cur_seg_blk    = 1;
	li->log_head	   = 1;
	li->n_inode	   = 0;
	li->seg_bitmap     = 0xffffffff;	

	// create a file of required size on disk that needs to be used to 
	// represent the lfs file system.	
	li->fd		 = open("/home/sphurti/lfslog",O_RDWR|O_CREAT);

	int file_size = (SEG_SIZE * MAX_NUM_SEG + BLKSIZE);
	char *buf = malloc(file_size);
	memset((void*)buf,0,file_size);
	pwrite(li->fd, buf, file_size,0);
	free(buf);

//	struct fuse_file_info *fi;
//	lfs_create("/tmp/fuse/hi");//,O_CREAT,fi);
	
}

static int lfs_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else
		res = -ENOENT;

	return res;
}
	
static int lfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	return 0;
}
// New file creation
int lfs_create(const char *path)//, mode_t mode,struct fuse_file_info *fi)
{
	fprintf(stderr, "\ninside the create func");
	struct segsum *ss;
	int j;
	struct file_inode_hash *s;
	HASH_FIND_STR(li->fih,path,s);

	// if the file already exists, do nothing
	if(s != NULL)
		return 0;
	
	// if the hash returns null, then create new file
	else {
		struct inode *i = (struct inode *)(li->cur_seg_buf + (li->cur_seg_blk * BLKSIZE));

		// initialise the inode
		i->ino = li->n_inode++;
		i->size = 0;
		
		// add the newly created inode to for given file into the hash table
		strcpy(s->f_name,path);
		s->inode_num = i->ino;
		HASH_ADD_STR(li->fih,f_name,s);
		
		//initialise  all the direct blk values to zero
		for(j = 0; j <= MAX_BLKS_FOR_FILE; j++)
		{
			i->direct[j].seg_num = 0;
			i->direct[j].blk_num = 0;
		}

		
		//initialise the inode map
		li->ino_map[i->ino].seg_num = li->log_head;
		li->ino_map[i->ino].blk_num = li->cur_seg_blk; 
		
		//initialise the segment summary
		ss = (struct segsum *)(li->cur_seg_buf);
		ss[li->cur_seg_blk].inode_num = i->ino;
		ss[li->cur_seg_blk].logical_blk = -1;


		// if the in-memory segment is full, write the data to disk
		if ( li->cur_seg_blk == (SEG_SIZE / BLKSIZE))
		{
			pwrite(li->fd, li->cur_seg_buf, SEG_SIZE, li->log_head * SEG_SIZE + BLKSIZE);
			
			// set the segment as allocated in bitmap
			li->seg_bitmap[li->log_head] = li->seg_bitmap[li->log_head] & ~(1 << li->log_head);

			li->cur_seg_blk = 0;
			li->log_head = (get_next_free_segment());
		}
		
		li->cur_seg_blk++;
	}
	return 0;
}

// file open operation
int lfs_open(const char *path, struct fuse_file_info *fi) 
{
	fprintf(stderr, "\ninside the open func");
	lfs_create(path);
	struct file_inode_hash *s;
	HASH_FIND_STR(li->fih,path,s);
	
	if(strcmp(s->f_name,path) != 0)
		return 0;
	else
		return -1;

}

int lfs_read(const char *path, char *buf, size_t count, off_t offset, struct fuse_file_info *fi)
{
}

static struct fuse_operations lfs_oper = {
    .getattr	= lfs_getattr,
    .readdir	= lfs_readdir,
    .open	= lfs_open,
    .create	= lfs_create,
    .read	= lfs_read,
};
/*
int main(int argc, char *argv[])
{
    lfs_init();
     
    fprintf(stderr, "\ninside the main func");
    return fuse_main(argc, argv, &lfs_oper,NULL);
}
*/
int main(int argc, char *argv[])
{
        struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
        struct fuse_chan *ch;
        char *mountpoint;
        int err = -1;

        if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
            (ch = fuse_mount(mountpoint, &args)) != NULL) {
                struct fuse_session *se;

                se = fuse_lowlevel_new(&args, &lfs_oper,
                                       sizeof(lfs_oper), NULL);
                if (se != NULL) {
                        if (fuse_set_signal_handlers(se) != -1) {
                                fuse_session_add_chan(se, ch);
                                err = fuse_session_loop(se);
                                fuse_remove_signal_handlers(se);
                                fuse_session_remove_chan(ch);
                        }
                        fuse_session_destroy(se);
                }
                fuse_unmount(mountpoint,ch);
        }
        fuse_opt_free_args(&args);

        return err ? 1 : 0;
}

