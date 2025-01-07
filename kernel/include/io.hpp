#pragma once
#include <cstdarg>

#include "reset.hpp"
#include "string.hpp"
#include "util.hpp"

int kputchar(int c);
int kgetchar();
void kputs(const char* s);
int kreadline(char* buffer, int length);
void kprint_hex(string_view view);
void kprint_str(string_view view);
void kprint(string_view view);

int PRINTF_FORMAT(1, 0) kvprintf(const char* format, va_list args);
int PRINTF_FORMAT(1, 2) kprintf(const char* format, ...);

void PRINTF_FORMAT(1, 2) klog(const char* format, ...);

#define panic(reason, ...)                                         \
  do {                                                             \
    klog("kernel panic: " reason "\n" __VA_OPT__(, ) __VA_ARGS__); \
    reset();                                                       \
  } while (0)
