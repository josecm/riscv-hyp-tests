
#include <stdio.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include "zynq_uart.h"

volatile Xil_Uart *uart = (void*)0xFF000000;

// void* memset(void* dest, int byte, size_t len)
// {
//   if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
//     uintptr_t word = byte & 0xFF;
//     word |= word << 8;
//     word |= word << 16;
//     word |= word << 16 << 16;

//     uintptr_t *d = dest;
//     while (d < (uintptr_t*)(dest + len))
//       *d++ = word;
//   } else {
//     char *d = dest;
//     while (d < (char*)(dest + len))
//       *d++ = byte;
//   }
//   return dest;
// }

// void* memcpy(void* dest, const void* src, size_t len)
// {
//   if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t)-1)) == 0) {
//     const uintptr_t* s = src;
//     uintptr_t *d = dest;
//     while (d < (uintptr_t*)(dest + len))
//       *d++ = *s++;
//   } else {
//     const char* s = src;
//     char *d = dest;
//     while (d < (char*)(dest + len))
//       *d++ = *s++;
//   }
//   return dest;
// }

int _read(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        ptr[i] = xil_uart_getc(uart);
    }

    return len;
}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        if (ptr[i] == '\n')
        {
            xil_uart_putc(uart, '\r');
        }
        xil_uart_putc(uart, ptr[i]);
    }

    return len;
}

int _lseek(int file, int ptr, int dir)
{
    //errno = ESPIPE;
    return -1;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    //errno = ENOTTY;
    return 0;
}


void* _sbrk(int increment)
{
    extern char _end; // From linker script
    static char* heap_end = &_end;

    char* current_heap_end = heap_end;
    heap_end += increment;
    
    return current_heap_end;
}

void _exit(int return_value)
{
    asm ("fence rw, rw" ::: "memory");
    while (1)
    {
        asm ("wfi" ::: "memory");
    }
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
    //errno = EINVAL;
    return -1;
}

void _init(){
    //xil_uart_init(uart);
    //xil_uart_enable(uart);
}