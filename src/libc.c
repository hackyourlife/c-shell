#include <libc.h>

int errno;

void* malloc(size_t size)
{
	size += sizeof(long);
	if(size & 4095) {
		size = (size & ~4095) + 4096;
	}

	void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if(ptr == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}

	long* sz = (long*) ptr;
	*sz = size;

	return &sz[1];
}

void free(void* ptr)
{
	long* sz = (long*) ptr - 1;

	if(munmap(sz, *sz) < 0) {
		perror("munmap");
	}
}

static int rngstate;

long random(void)
{
	rngstate = (rngstate * 1103515245) + 12345;
	return rngstate;
}
