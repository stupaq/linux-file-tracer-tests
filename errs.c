// MAKE MAIN

#include "gen.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char buf[0x4000];

int main() {
	open("file_ro", O_WRONLY);

	int fdw = open("file_ro", O_WRONLY);
	read(fdw, buf, 100);

	int fdr = open("file_ro", O_RDONLY);
	write(fdr, buf, 100);

	lseek(fdr, -12312, SEEK_CUR);

	close(fdw);
	close(fdr);
}
