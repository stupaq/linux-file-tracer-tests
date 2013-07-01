#ifndef GEN_H
#define GEN_H

#include <stdlib.h>

static inline void gen(char *buf, size_t len) {
	unsigned short state[3] = { 0x1234, 0x5678, 0x9abc };
	for (int i = 0; i < len; i++) {
		buf[i] = jrand48(state);
	}
}

#endif  /* GEN_H */
