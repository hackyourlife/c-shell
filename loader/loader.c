#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	if(argc != 2) {
		printf("Usage: %s shellcode.bin\n", *argv);
		return 1;
	}

	const char* filename = argv[1];
	struct stat buf;
	if(stat(filename, &buf) < 0) {
		perror("stat");
		return 1;
	}

	printf("Shellcode size: %lu\n", buf.st_size);

	size_t size = buf.st_size;
	if(size & 0xFFF) {
		size += 0x1000 - (size & 0xFFF);
	}

	void* shellcode = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
	if(shellcode == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	printf("Sellcode address: %p (%lu bytes)\n", shellcode, size);

	FILE* in = fopen(filename, "rb");
	if(fread(shellcode, buf.st_size, 1, in) < 1) {
		printf("Failed to read shellcode\n");
		return 1;
	}
	fclose(in);

	void (*fn)(void) = (void (*)(void)) shellcode;
	fn();

	return 0;
}
