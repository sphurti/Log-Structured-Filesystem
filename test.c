#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


int main()
{
	int fd = open("/tmp/fuse/hi", O_RDWR);
	assert(fd > 0);
	close(fd);
}
