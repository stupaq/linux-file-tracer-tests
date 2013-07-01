// MAKE MAIN

#include "gen.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char buf[0x400000];

#define NMUX 8
#define CHUNKSIZE 0x4000

int main() {
	srand(1212423);
	char tmp[256];
	int fd[NMUX];
	for (int i = 0; i < NMUX; i++) {
		sprintf(tmp, "file%d", i);
		fd[i] = open(tmp, O_RDWR);
		if (fd[i] < 0) {
			perror("open");
			return 1;
		}
	}
	gen(buf, sizeof(buf));
	int pos[NMUX] = { 0 };
	while (1) {
		int nfree = 0;
		for (int i = 0; i < NMUX; i++) {
			if (pos[i] == sizeof(buf)) {
				nfree++;
				continue;
			}
			if (rand() & 1)
				continue;
			size_t len = rand() % CHUNKSIZE + 1;
			int rnd = rand() % 10;
			if (rnd >= 4) {
				if (pos[i] + len > sizeof buf)
					len = sizeof buf - pos[i];
				if (write(fd[i], buf + pos[i], len) != len) {
					perror("write");
					return 1;
				}
				pos[i] += len;
			} else if (rnd < 4 && rnd >= 1) {
				if (pos[i] < len)
					len = pos[i];
				if (lseek(fd[i], -len, SEEK_CUR) < 0) {
					perror("lseek");
					return 1;
				}
				pos[i] -= len;
			} else if (rnd < 1) {
				char rbuf[CHUNKSIZE];
				ssize_t r = 1;
				while (r) {
					if ((r = read(fd[i], rbuf, len)) < 0) {
						perror("read");
						return 1;
					}
					pos[i] += r;
				}
			}
		}
		if (nfree == NMUX)
			break;
	}
	return 0;
}
