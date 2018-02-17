#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>

#define NAME "eudyptula"
#define PATH "/dev/eudyptula"
#define ME "siqueira"
#define LIMIT 256

static char lazy_string[LIMIT];

int main(int argc, char ** argv)
{
	int retval = 0, fd = 0;

	printf("Hey dude, I gona test your eudyptula misc char! :)\n");

	fd = open(PATH, O_RDWR);
	if (fd < 0) {
		printf("Something failed on open!\n");
		return fd;
	}

	retval = read(fd, lazy_string, LIMIT);
	
	printf("We read >> %s << and the total of >> %d << bytes\n",
		lazy_string, retval);

	printf("----\n");
	retval = write(fd, ME, sizeof(ME));
	if (retval < 0)
		printf("Invalid value\n");
	else
		printf("Valid\n");

	return retval;
}
