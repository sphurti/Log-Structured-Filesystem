CC     = gcc
CFLAGS = -g -Wall -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26
LFLAGS = -lfuse
OBJS = segment.o  inode.o lfs.o

all: lfs

lfs: $(OBJS)
	$(CC) $(OBJS) -o lfs $(LFLAGS)

segment.o: segment.c segment.h
	$(CC) $(CFLAGS) -c -o $@ $<

inode.o: inode.c inode.h
	$(CC) $(CFLAGS) -c -o $@ $<

lfs.o: lfs.c lfs.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f lfs *.o

# This is GNU makefile extension to notify that roughly means: 'clean' does
# not depend on any files in order to call it.
.PHONY: clean
