#ifndef TTY_H
#define TTY_H

#define TAB_SIZE 4

void putc(char c);
void puts(const char* s);
void printf(const char* fmt, ...);

#ifdef GRAPHICS
#else

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#endif

#endif // #ifndef TTY_H
