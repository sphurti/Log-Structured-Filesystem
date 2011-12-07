#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>


int main()
{
	int fd = open("/tmp/fuse/hi", O_CREAT | O_RDWR);
	if (fd < 0)
		fprintf(stderr, "%d\n", errno);
	assert(fd > 0);
	close(fd);
}
