#include <libc.h>

static char* __sprinth(char* result, int resultlen, unsigned int x)
{
	const char* letters = "0123456789ABCDEF";
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = letters[x & 0x0F];
		x >>= 4;
	}
	return p;
}

static char* __sprinto(char* result, int resultlen, unsigned int x)
{
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = x & 0x07;
		x >>= 3;
	}
	return p;
}

static char* __sprintd(char* result, int resultlen, unsigned int x)
{
	int neg = 0;
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	if((int) x < 0) {
		neg = 1;
		x = -x;
	}
	while(x != 0) {
		*(--p) = (x % 10) + 0x30;
		x /= 10;
	}
	if(neg)
		*(--p) = '-';
	return p;
}

static char* __sprintu(char* result, int resultlen, unsigned int x)
{
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = (x % 10) + 0x30;
		x /= 10;
	}
	return p;
}

#ifndef LONG32
static char* __sprinth64(char* result, u32 resultlen, u64 x)
{
	const char* letters = "0123456789ABCDEF";
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = letters[x & 0x0F];
		x >>= 4;
	}
	return p;
}

static char* __sprintd64(char* result, u32 resultlen, u64 x)
{
	int neg = 0;
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	if((s64) x < 0) {
		neg = 1;
		x = -x;
	}
	while(x != 0) {
		*(--p) = (x % 10) + 0x30;
		x /= 10;
	}
	if(neg)
		*(--p) = '-';
	return p;
}

static char* __sprintu64(char* result, u32 resultlen, u64 x)
{
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = (x % 10) + 0x30;
		x /= 10;
	}
	return p;
}
#else
static char* __sprinth32(char* result, unsigned int resultlen, u32 x)
{
	const char* letters = "0123456789ABCDEF";
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = letters[x & 0x0F];
		x >>= 4;
	}
	return p;
}

static char* __sprintd32(char* result, unsigned int resultlen, u32 x)
{
	u32 q, r;

	int neg = 0;
	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	if((s32) x < 0) {
		neg = 1;
		x = -x;
	}
	while(x != 0) {
		*(--p) = (x % 10LL) + 0x30;
		x /= 10LL;
	}
	if(neg)
		*(--p) = '-';
	return p;
}

static char* __sprintu32(char* result, unsigned int resultlen, u32 x)
{
	u32 q, r;

	char* p = &result[resultlen - 1];
	*p = 0;
	if(x == 0) {
		*(--p) = '0';
		return p;
	}
	while(x != 0) {
		*(--p) = (x % 10LL) + 0x30;
		x /= 10LL;
	}
	return p;
}
#endif

#define TMP_SIZE 32

int vsprintf(char* buf, const char* s, va_list args)
{
	char tmp[TMP_SIZE];
	char* c;
	char* p = buf;
	unsigned int n;
	char pad = 0;
	int t;
	for(; *s != 0; s++) {
		if(*s == '%') {
			pad = 0;
			n = 0;
			s++;
			if(*s == '0') {
				s++;
				pad = '0';
			}
			while((*s >= '0') && (*s <= '9')) {
				n *= 10;
				n += *s - '0';
				s++;
				if(!pad)
					pad = ' ';
			}
			if(*s == 'c') {
				int val = va_arg(args, int);
				*(p++) = val;
			} else if(*s == 'd') {
				int val = va_arg(args, int);
				c = __sprintd(tmp, TMP_SIZE, val);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = pad;
				for(; *c != 0; c++)
					*(p++) = *c;
			} else if(*s == 'u') {
				unsigned int val = va_arg(args, unsigned int);
				c = __sprintu(tmp, TMP_SIZE, val);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = pad;
				for(; *c != 0; c++)
					*(p++) = *c;
			} else if(*s == 'x') {
				int val = va_arg(args, int);
				c = __sprinth(tmp, TMP_SIZE, val);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = pad;
				for(; *c != 0; c++)
					*(p++) = *c | 0x20;
			} else if(*s == 'X') {
				int val = va_arg(args, int);
				c = __sprinth(tmp, TMP_SIZE, val);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = pad;
				for(; *c != 0; c++)
					*(p++) = *c;
			} else if(*s == 'o') {
				int val = va_arg(args, int);
				c = __sprinto(tmp, TMP_SIZE, val);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = pad;
				for(; *c != 0; c++)
					*(p++) = *c;
			} else if(*s == 'l') {
				s++;
				if(*s == 'd') {
#ifdef LONG32
					u32 val = va_arg(args, u32);
					c = __sprintd32(tmp, TMP_SIZE, val);
#else
					u64 val = va_arg(args, u64);
					c = __sprintd64(tmp, TMP_SIZE, val);
#endif
					t = n - strlen(c);
					if(pad)
						for(; t > 0; t--)
							*(p++) = pad;
					for(; *c != 0; c++)
						*(p++) = *c;
				} else if(*s == 'u') {
#ifdef LONG32
					u32 val = va_arg(args, u32);
					c = __sprintu32(tmp, TMP_SIZE, val);
#else
					u64 val = va_arg(args, u64);
					c = __sprintu64(tmp, TMP_SIZE, val);
#endif
					t = n - strlen(c);
					if(pad)
						for(; t > 0; t--)
							*(p++) = pad;
					for(; *c != 0; c++)
						*(p++) = *c;
				} else if(*s == 'x') {
#ifdef LONG32
					u32 val = va_arg(args, u32);
					c = __sprinth(tmp, TMP_SIZE, val);
#else
					u64 val = va_arg(args, u64);
					c = __sprinth64(tmp, TMP_SIZE, val);
#endif
					t = n - strlen(c);
					if(pad)
						for(; t > 0; t--)
							*(p++) = pad;
					for(; *c != 0; c++)
						*(p++) = *c | 0x20;
				} else if(*s == 'X') {
#ifdef LONG32
					u32 val = va_arg(args, u32);
					c = __sprinth32(tmp, TMP_SIZE, val);
#else
					u64 val = va_arg(args, u64);
					c = __sprinth64(tmp, TMP_SIZE, val);
#endif
					t = n - strlen(c);
					if(pad)
						for(; t > 0; t--)
							*(p++) = pad;
					for(; *c != 0; c++)
						*(p++) = *c;
				}
			} else if(*s == 's') {
				c = (char*)va_arg(args, char*);
				t = n - strlen(c);
				if(pad)
					for(; t > 0; t--)
						*(p++) = ' ';
				for(; *c != 0; c++)
					*(p++) = *c;
			} else if(*s == '%') {
				*(p++) = '%';
			}
		} else
			*(p++) = *s;
	}
	*(p++) = 0;
	return p - buf - 1;
}

int sprintf(char* buf, const char* format, ...)
{
	va_list args;
	int length;
	va_start(args, format);
	length = vsprintf(buf, format, args);
	va_end(args);

	return length;
}

int printf(const char* format, ...)
{
	va_list args;
	int length;
	char buf[128];
	va_start(args, format);
	length = vsprintf(buf, format, args);
	va_end(args);

	return write(1, buf, length);
}

int print(const char* s)
{
	size_t len = strlen(s);
	return write(1, s, len);
}

int puts(const char* s)
{
	size_t len = strlen(s);
	size_t result = write(1, s, len);
	result += write(1, "\n", 1);
	return result;
}

#ifdef USE_ERRNO
void perror(const char* msg)
{
	printf("%s: %d\n", msg, errno);
}
#endif
