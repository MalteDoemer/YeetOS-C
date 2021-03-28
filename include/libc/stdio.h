#pragma once

#include "stddef.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int FILE;
typedef long long fpos_t;

int fclose(FILE* stream);
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
int fflush(FILE* stream);
int fgetpos(FILE* stream, fpos_t* pos);
FILE* fopen(const char* filename, const char* mode);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
FILE* freopen(const char* filename, const char* mode, FILE* stream);
int fseek(FILE* stream, long int offset, int whence);
int fsetpos(FILE* stream, const fpos_t* pos);
long int ftell(FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int remove(const char* filename);
int rename(const char* old_filename, const char* new_filename);
void rewind(FILE* stream);
void setbuf(FILE* stream, char* buffer);
int setvbuf(FILE* stream, char* buffer, int mode, size_t size);
FILE* tmpfile(void);
char* tmpnam(char* str);
int fprintf(FILE* stream, const char* format, ...);
int printf(const char* fmt, ...);
int vprintf(const char* fmt, va_list va);
int sprintf(char* buffer, const char* fmt, ...);
int snprintf(char* buffer, size_t count, const char* fmt, ...);
int vsprintf(char* buffer, const char* fmt, va_list va);
int vsnprintf(char* buffer, size_t count, const char* fmt, va_list va);
int fscanf(FILE* stream, const char* format, ...);
int scanf(const char* format, ...);
int sscanf(const char* str, const char* format, ...);
int fgetc(FILE* stream);
char* fgets(char* str, int n, FILE* stream);
int fputc(int chr, FILE* stream);
int fputs(const char* str, FILE* stream);
int getc(FILE* stream);
int getchar(void);
char* gets(char* str);
int putc(int chr, FILE* stream);
int putchar(int chr);
int puts(const char* str);
int ungetc(int chr, FILE* stream);
void perror(const char* str);

#ifdef __cplusplus
}
#endif