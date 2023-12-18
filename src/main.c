#include <libc.h>

/* this program is a very simple and minimal "ls" */

void printu(unsigned int x)
{
	char result[10];
	char* p = &result[sizeof(result) - 1];

	for(unsigned int i = 0; i < sizeof(result); i++) {
		result[i] = ' ';
	}

	*p = 0;

	if(x == 0) {
		*(--p) = '0';
		write(1, result, sizeof(result));
	}

	while(x != 0) {
		*(--p) = (x % 10) + 0x30;
		x /= 10;
	}

	write(1, result, sizeof(result));
}

void print_flag(unsigned int flag, unsigned int n)
{
	unsigned int perm = flag >> (n * 3);
	char buf[3] = { '-', '-', '-' };
	if(perm & 1) {
		buf[2] = 'x';
	}
	if(perm & 2) {
		buf[1] = 'w';
	}
	if(perm & 4) {
		buf[0] = 'r';
	}
	write(1, buf, 3);
}

void space(void)
{
	write(1, " ", 1);
}

ENTRY()
{
	char buf[512];

	print("Directory listing: ");
	char* result = getcwd(buf, sizeof(buf));
	if((long) result > 0) {
		puts(buf);
	} else {
		print("\n");
	}
	int fd = open(".", O_RDONLY | O_DIRECTORY);

	long n;
	while((n = getdents(fd, (struct dirent*) buf, sizeof(buf))) > 0) {
		struct dirent* d = (struct dirent*) buf;
		while((long) ((uintptr_t) d - (uintptr_t) buf) < n) {
			const char* name = d->d_name;

			struct stat statbuf;
			stat(name, &statbuf);

			char type = '?';
			switch(statbuf.st_mode & S_IFMT) {
				case S_IFSOCK:
					type = 's';
					break;
				case S_IFLNK:
					type = 'l';
					break;
				case S_IFREG:
					type = '-';
					break;
				case S_IFBLK:
					type = 'b';
					break;
				case S_IFDIR:
					type = 'd';
					break;
				case S_IFCHR:
					type = 'c';
					break;
				case S_IFIFO:
					type = 'f';
					break;
			}

			write(1, &type, 1);
			print_flag(statbuf.st_mode, 2);
			print_flag(statbuf.st_mode, 1);
			print_flag(statbuf.st_mode, 0);
			space();
			printu((unsigned int) statbuf.st_size);
			space();
			puts(name);

			d = (struct dirent*) ((char*) d + d->d_reclen);
		}
	}

	close(fd);

#if 0
	/* need some shell? */
	char* argv[] = { "/bin/sh", NULL };
	char* envp[] = { "PATH=/bin", NULL };
	execve(argv[0], argv, envp);
#endif

	exit(0);
}
