#pragma once
#include <cstdarg>

#include "util.hpp"

int kputchar(int c);
int kgetchar();
void kputs(const char* s);
int kreadline(char* buffer, int length);

int PRINTF_FORMAT(1, 0) kvprintf(const char* format, va_list args);
int PRINTF_FORMAT(1, 2) kprintf(const char* format, ...);
