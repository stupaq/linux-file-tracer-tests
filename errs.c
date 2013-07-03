// MAKE MAIN

#include "gen.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char buf[0x4000];

int main() {
	int fdw, fdr;

	assert(open("file_ro", O_WRONLY) < 0);

	assert((fdw = open("file_wo", O_WRONLY)) >= 0);
	assert(read(fdw, buf, 100) < 0);

	assert((fdr = open("file_ro", O_RDONLY)) >= 0);
	assert(write(fdr, buf, 100) < 0);

	assert(lseek(fdr, -12312, SEEK_CUR) < 0);

	assert(read(fdr, NULL, 100) < 0);
	assert(write(fdw, NULL, 100) < 0);

	close(fdw);
	close(fdr);
}
