CC     = gcc

all: lfs_test

lfs_test: lfs_test.c
	$(CC) lfs_test.c -o lfs_test

clean:
	rm -f lfs_test

# This is GNU makefile extension to notify that roughly means: 'clean' does
# not depend on any files in order to call it.
.PHONY: clean
