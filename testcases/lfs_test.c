#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main()
{

	
	char *bufread,*buff,*buf ;
        int fd,fd1,n1,r;
       
	 
	printf("\n\n*********************************************TESTCASE-1**********************************\n\n");
	// test writing of few number of bytes on to file
	printf("\n-------------------- Test writing fewer number of bytes to file -----------------------\n");
	buf = "There are several industry associations and hardware conferences devoted to maintaining and improving support for diverse hardware under Linux, such as FreedomHEC.";
	fd = open("/tmp/fuse/hello",  O_CREAT | O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = write(fd,buf,strlen(buf));
        printf("\n\n\nNumber of bytes written to file : %d\n",r);
        close(fd);

	// test reading of file with fewer number of bytes
	printf("-------------------- Test reading of file containing fewer number of bytes---------------\n");
	bufread = (char*)malloc(163);
        fd = open("/tmp/fuse/hello",  O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = pread(fd, bufread, 163,0);
        printf("\n\nNumber of bytes read from file : %d \nContent of file :\n %s\n",r,bufread);
        free(bufread);
        close(fd);



	printf("\n\n***********************************************TESTCASE-2****************************\n\n");

	// test writing of number of bytes greater than block size to file
	
	printf("\n-------------------- Test writing data more then block size to the file -----------------------\n");
	fd1 = open("./test.txt", O_RDWR);
	buff = malloc(5096);
	n1 = read(fd1,buff,5096);
        fd = open("/tmp/fuse/file1",  O_CREAT | O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = write(fd,buff,strlen(buff));
        printf("\n\n\nNumber of bytes written to file : %d\n",r);
	free(buff);
        close(fd);
       
	// test reading of file with data greater than block size 
	
	printf("-------------------- Test reading of file having more then block data---------------\n");
	bufread = (char*)malloc(5024);
        fd = open("/tmp/fuse/file1",  O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = pread(fd, bufread, 5024,0);
        printf("\n\nNumber of bytes read from file : %d \nContent of file :\n %s\n",r,bufread);
	free(bufread);
        close(fd);

	
	printf("\n\n**********************************************TESTCASE-3*********************************\n\n");
	// test writing of number of bytes greater than block size

	printf("\n-------------------- Test writing data more than two block size to the file -----------------------\n");
        fd1 = open("./huge_file.txt", O_RDWR);
        buff = malloc(9096);
        n1 = read(fd1,buff,9096);
        fd = open("/tmp/fuse/file2",  O_CREAT | O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = write(fd,buff,strlen(buff));
        printf("\n\n\nNumber of bytes written to file : %d\n",r);
        free(buff);
        close(fd);

        // test reading of file with data greater than block size 
	
	printf("-------------------- Test reading of file having more than 2 block---------------\n");
        bufread = (char*)malloc(9096);
        fd = open("/tmp/fuse/file2",  O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = pread(fd, bufread, 9096,0);
        printf("\n\nNumber of bytes read from file : %d \nContent of file :\n %s\n",r,bufread);
        free(bufread);
        close(fd);

	
	printf("\n\n*******************************************TESTCASE-4********************************\n\n");
	// testcase to overwrite the exisitng file
	
	printf("\n-------------------- Test overwriting the exisiting file -----------------------\n");
	fd1 = open("./test.txt", O_RDWR);
        buff = malloc(3096);
        n1 = read(fd1,buff,3096);
        fd = open("/tmp/fuse/file2",  O_CREAT | O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = write(fd,buff,strlen(buff));
        printf("\n\n\nNumber of bytes written to file : %d\n",r);
        free(buff);
        close(fd);

        // test reading of file for overwritten file
	
	printf("-------------------- Test reading the overwritten file---------------\n");
        bufread = (char*)malloc(1300);
        fd = open("/tmp/fuse/file2",  O_RDWR);
        if (fd < 0)
                fprintf(stderr, "%d\n", errno);
        assert(fd > 0);
        r = pread(fd, bufread, 1300, 400);
        printf("\n\nNumber of bytes read from file : %d \nContent of file :\n %s\n",r,bufread);
        free(bufread);
        close(fd);
	

	
	printf("\n\n**********************************************TESTCASE-5********************************\n\n");
	printf("-------------------- Test deleting the exisiting file---------------\n");
	r = unlink("/tmp/fuse/hello");
        if (r < 0)
                fprintf(stderr, "%d\n", errno);

	
	printf("-------------------- Test opening the deleted file---------------\n");
        fd = open("/tmp/fuse/hello", O_RDWR);
        if (fd < 0)
		if(errno == 2)
                fprintf(stderr, "%d : No such file or directory\n", errno);

        close(fd);


}
