#ifndef __NANOLIBC_H__
#define __NANOLIBC_H__

#include <types.h>
#include <stdarg.h>
#include <stddef.h>

#if defined(__x86_64__)
#define LONG64		1
#define	PTRSZ		uint64_t

#define	SYS_read	0
#define	SYS_write	1
#define	SYS_open	2
#define	SYS_close	3
#define	SYS_stat	4
#define	SYS_fstat	5
#define	SYS_lstat	6
#define	SYS_lseek	8
#define	SYS_mmap	9
#define SYS_mprotect	10
#define	SYS_munmap	11
#define	SYS_readv	19
#define	SYS_writev	20
#define	SYS_execve	59
#define	SYS_exit	60
#define	SYS_uname	63
#define	SYS_getdents	78
#define	SYS_getcwd	79
#define	SYS_mkdir	83
#define	SYS_rmdir	84
#define	SYS_getuid	102
#define	SYS_getgid	104
#define	SYS_geteuid	107
#define	SYS_getegid	108
#define	SYS_exit_group	231
#else
#error "Unsupported architecture"
#endif

#define	__ssc(x) ((PTRSZ) (x))

#if defined(__x86_64__)
#define	O_ACCMODE	00000003
#define	O_RDONLY	00000000
#define	O_WRONLY	00000001
#define	O_RDWR		00000002
#define	O_CREAT		00000100
#define	O_EXCL		00000200
#define	O_NOCTTY	00000400
#define	O_TRUNC		00001000
#define	O_APPEND	00002000
#define	O_NONBLOCK	00004000
#define	O_DSYNC		00010000
#define	FASYNC		00020000
#define	O_DIRECT	00040000
#define	O_LARGEFILE	00100000
#define	O_DIRECTORY	00200000
#define	O_NOFOLLOW	00400000
#define	O_NOATIME	01000000
#define	O_CLOEXEC	02000000

#define	MAP_FAILED	((void *) -1)

#define	PROT_NONE	0x0
#define	PROT_READ	0x1
#define	PROT_WRITE	0x2
#define	PROT_EXEC	0x4

#define	MAP_SHARED	0x01
#define	MAP_PRIVATE	0x02
#define	MAP_TYPE	0x0f
#define	MAP_FIXED	0x10
#define	MAP_ANONYMOUS	0x20

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#define	EBADF		9
#define	EINVAL		22
#define	ENOSYS		38
#endif

#define	DT_UNKNOWN	0
#define	DT_FIFO		1
#define	DT_CHR		2
#define	DT_DIR		4
#define	DT_BLK		6
#define	DT_REG		8
#define	DT_LNK		10
#define	DT_SOCK		12
#define	DT_WHT		14

#define	S_IFMT		00170000
#define	S_IFSOCK	 0140000
#define	S_IFLNK		 0120000
#define	S_IFREG		 0100000
#define	S_IFBLK		 0060000
#define	S_IFDIR		 0040000
#define	S_IFCHR		 0020000
#define	S_IFIFO		 0010000
#define	S_ISUID		 0004000
#define	S_ISGID		 0002000
#define	S_ISVTX		 0001000

#define	S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define	S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

#define	S_IRWXU		00700
#define	S_IRUSR		00400
#define	S_IWUSR		00200
#define	S_IXUSR		00100

#define	S_IRWXG		00070
#define	S_IRGRP		00040
#define	S_IWGRP		00020
#define	S_IXGRP		00010

#define	S_IRWXO		00007
#define	S_IROTH		00004
#define	S_IWOTH		00002
#define	S_IXOTH		00001

typedef int		mode_t;
typedef long		ssize_t;
typedef long		off_t;

struct utsname {
	char		sysname[65];
	char		nodename[65];
	char		release[65];
	char		version[65];
	char		machine[65];
	char		domainname[65];
};

struct dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char		d_name[];
	/* char		d_type; // offset is (d_reclen - 1) */
};

struct iovec {
	void*		iov_base;
	size_t		iov_len;
};

struct stat {
	unsigned long	st_dev;
	unsigned long	st_ino;
	unsigned long	st_nlink;

	unsigned int	st_mode;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	__pad0;
	unsigned long	st_rdev;
	long		st_size;
	long		st_blksize;
	long		st_blocks; /* Number 512-byte blocks allocated */

	unsigned long	st_atime;
	unsigned long	st_atime_nsec;
	unsigned long	st_mtime;
	unsigned long	st_mtime_nsec;
	unsigned long	st_ctime;
	unsigned long	st_ctime_nsec;
	long		__unused[3];
};

#ifdef USE_ERRNO
extern int errno;
#endif

#define	ENTRY() __attribute__((section(".entrypoint"))) \
	_Noreturn void _start(void)

/* syscall helpers */
#ifdef __x86_64__
static inline long __syscall0(long id)
{
	long result;
	__asm__ volatile("syscall" : "=a"(result)
				   : "a"(id)
				   : "memory", "rcx", "r11");
	return result;
}

static inline long __syscall1(long id, long a1)
{
	long result;
	__asm__ volatile("syscall" : "=a"(result)
				   : "a"(id), "D"(a1)
				   : "memory", "rcx", "r11");
	return result;
}

static inline long __syscall2(long id, long a1, long a2)
{
	long result;
	__asm__ volatile("syscall" : "=a"(result)
				   : "a"(id), "D"(a1), "S"(a2)
				   : "memory", "rcx", "r11");
	return result;
}

static inline long __syscall3(long id, long a1, long a2, long a3)
{
	long result;
	__asm__ volatile("syscall" : "=a"(result)
				   : "a"(id), "D"(a1), "S"(a2), "d"(a3)
				   : "memory", "rcx", "r11");
	return result;
}

static inline long __syscall6(long id, long a1, long a2, long a3, long a4,
		long a5, long a6)
{
	long result;
	register int64_t r10 asm("r10") = a4;
	register int64_t r8 asm("r8") = a5;
	register int64_t r9 asm("r9") = a6;
	__asm__ volatile("syscall" : "=a"(result)
				   : "a"(id), "D"(a1), "S"(a2), "d"(a3),
				     "r"(r10), "r"(r8), "r"(r9)
				   : "memory", "rcx", "r11");
	return result;
}
#endif

#define __SYSCALL_0(n) \
	__syscall0(__ssc(n));

#define __SYSCALL_1(n, a) \
	__syscall1(__ssc(n), __ssc(a));

#define __SYSCALL_2(n, a, b) \
	__syscall2(__ssc(n), __ssc(a), __ssc(b));

#define __SYSCALL_3(n, a, b, c) \
	__syscall3(__ssc(n), __ssc(a), __ssc(b), __ssc(c));

#define __SYSCALL_4(n, a, b, c, d) \
	__syscall4(__ssc(n), __ssc(a), __ssc(b), __ssc(c), __ssc(d));

#define __SYSCALL_5(n, a, b, c, d, e) \
	__syscall5(__ssc(n), __ssc(a), __ssc(b), __ssc(c), __ssc(d), __ssc(e));

#define __SYSCALL_6(n, a, b, c, d, e, f) \
	__syscall6(__ssc(n), __ssc(a), __ssc(b), __ssc(c), __ssc(d), __ssc(e), __ssc(f));

#ifdef USE_ERRNO
#define __SYSCALL_RET(result) { \
	if(result < 0) { \
		errno = -result; \
		return -1; \
	} \
	return result; \
}
#else
#define __SYSCALL_RET(result) { \
	return result; \
}
#endif

#define __SYSCALL_0P(id) { \
	long result = __SYSCALL_0(id); \
	__SYSCALL_RET(result); \
}

#define __SYSCALL_1P(id, a1) { \
	long result = __SYSCALL_1(id, a1); \
	__SYSCALL_RET(result); \
}

#define __SYSCALL_2P(id, a1, a2) { \
	long result = __SYSCALL_2(id, a1, a2); \
	__SYSCALL_RET(result); \
}

#define __SYSCALL_3P(id, a1, a2, a3) { \
	long result = __SYSCALL_3(id, a1, a2, a3); \
	__SYSCALL_RET(result); \
}

#define __SYSCALL_6P(id, a1, a2, a3, a4, a5, a6) { \
	long result = __SYSCALL_6(id, a1, a2, a3, a4, a5, a6); \
	__SYSCALL_RET(result); \
}

/* posix/libc functions */
static inline ssize_t read(int fd, void *buf, size_t count)
{
	__SYSCALL_3P(SYS_read, fd, buf, count);
}

static inline ssize_t write(int fd, const void *buf, size_t count)
{
	__SYSCALL_3P(SYS_write, fd, buf, count);
}

static inline int open(const char *filename, int flags)
{
	__SYSCALL_3P(SYS_open, filename, flags, 0644);
}

static inline int close(int fd)
{
	__SYSCALL_1P(SYS_close, fd);
}

static inline long lseek(int fd, off_t offset, int whence)
{
	__SYSCALL_3P(SYS_lseek, fd, offset, whence);
}

static inline ssize_t readv(int fd, const struct iovec* iov, int iovcnt)
{
	__SYSCALL_3P(SYS_readv, fd, iov, iovcnt);
}

static inline ssize_t writev(int fd, const struct iovec* iov, int iovcnt)
{
	__SYSCALL_3P(SYS_writev, fd, iov, iovcnt);
}

static inline char* getcwd(char* buf, size_t size)
{
	int64_t result = __SYSCALL_2(SYS_getcwd, buf, size);
#ifdef USE_ERRNO
	if(result < 0) {
		errno = -result;
		return NULL;
	}
	return buf;
#else
	return (char*) result;
#endif
}

static inline _Noreturn void _Exit(int ec)
{
	__SYSCALL_1(SYS_exit_group, ec);
	while(1);
}

static inline _Noreturn void exit(int ec)
{
	__SYSCALL_1(SYS_exit, ec);
	while(1);
}

static inline int mkdir(const char *path, mode_t mode)
{
	__SYSCALL_2P(SYS_mkdir, path, mode);
}

static inline int rmdir(const char *path, mode_t mode)
{
	__SYSCALL_2P(SYS_rmdir, path, mode);
}

static inline int uname(struct utsname *buf)
{
	__SYSCALL_1P(SYS_uname, buf);
}

static inline int getuid(void)
{
	__SYSCALL_0P(SYS_getuid);
}

static inline int getgid(void)
{
	__SYSCALL_0P(SYS_getgid);
}

static inline int geteuid(void)
{
	__SYSCALL_0P(SYS_geteuid);
}

static inline int getegid(void)
{
	__SYSCALL_0P(SYS_getegid);
}

static inline void* mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
	long result = __SYSCALL_6(SYS_mmap, addr, len, prot, flags, fildes, off);
#ifdef USE_ERRNO
	if(result < 0) {
		errno = -result;
		return MAP_FAILED;
	}
#endif
	return (void*) result;
}

static inline int munmap(void *addr, size_t len)
{
	__SYSCALL_2P(SYS_munmap, addr, len);
}

static inline int mprotect(void *addr, size_t len, int prot)
{
	__SYSCALL_3P(SYS_mprotect, addr, len, prot);
}

static inline long getdents(unsigned int fd, struct dirent* dirp, unsigned int count)
{
	__SYSCALL_3P(SYS_getdents, fd, dirp, count);
}

static inline int execve(const char* pathname, char* const argv[], char* const envp[])
{
	__SYSCALL_3P(SYS_execve, pathname, argv, envp);
}

static inline int stat(const char* restrict pathname,
		struct stat* restrict statbuf)
{
	__SYSCALL_2P(SYS_stat, pathname, statbuf);
}

static inline int fstat(int fd, struct stat* statbuf)
{
	__SYSCALL_2P(SYS_fstat, fd, statbuf);
}

static inline int lstat(const char* restrict pathname,
		struct stat* restrict statbuf)
{
	__SYSCALL_2P(SYS_lstat, pathname, statbuf);
}


/* pure userspace functions */
static inline int strlen(const char *s)
{
	char *p = (char*) s;
	for (; *p; p++)
		;
	return p - s;
}

static inline void *memset(void *s, int c, size_t n)
{
	char *p = (char*) s;
	for(size_t i = 0; i < n; i++)
		*(p++) = (char) c;
	return s;
}

static inline void *memcpy(void *dst, const void *src, size_t n)
{
	char *d = (char*) dst;
	char *s = (char*) src;
	for(size_t i = 0; i < n; i++)
		*(d++) = *(s++);
	return dst;
}

static inline int memcmp(const void *s1, const void *s2, size_t n)
{
	char *a = (char*) s1;
	char *b = (char*) s2;
	while(n--) {
		if(*a != *b)
			return *a - *b;
		a++;
		b++;
	}
	return 0;
}

int	puts(const char *s);
int	print(const char* s);
int	sprintf(char *buf, const char *fmt, ...);
int	printf(const char *fmt, ...);
#ifdef USE_ERRNO
void	perror(const char* msg);
#endif

void*	malloc(size_t size);
void	free(void *ptr);

long random(void);

#define abort()	__abort(__FILE__, __LINE__)

static inline void __abort(const char* filename, int line)
{
	printf("abort: %s:%d\n", filename, line);
	exit(139);
}

#endif
