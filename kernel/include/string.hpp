#pragma once

#include <concepts>

#include "util.hpp"

extern "C" {
void memzero(void* start, void* end);
void* memcpy(void* dst, const void* src, size_t n);
void memset(void* b, int c, size_t len);
}

int memcmp(const void* s1, const void* s2, size_t n);
size_t strlen(const char* s);
size_t strnlen(const char* s, size_t n);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
const char* strchr(const char* s, char c);
const char* strchr_or_e(const char* s, char c);
long strtol(const char* s, const char** endptr = nullptr, size_t base = 0,
            size_t n = 0);
bool isprint(char c);

class string_view {
  const char* buf_;
  size_t size_;

 public:
  using iterator = const char*;
  string_view() : buf_(nullptr), size_(0) {}
  string_view(const char* buf) : buf_(buf), size_(strlen(buf)) {}
  template <typename T>
    requires std::is_convertible_v<T, size_t>
  string_view(const char* buf, T size) : buf_(buf), size_(size) {}
  iterator begin() {
    return buf_;
  }
  iterator end() {
    return buf_ + size_;
  }
  const char* data() {
    return buf_;
  }
  size_t size() const {
    return size_;
  }
  bool empty() const {
    return size_ == 0;
  }
  char operator[](size_t i) const {
    return buf_[i];
  }
  bool printable() const {
    for (size_t i = 0; i < size_; i++) {
      auto c = buf_[i];
      if (not isprint(c) and not(i + 1 == size_ and c == 0))
        return false;
    }
    return true;
  }
};

bool operator==(string_view a, string_view b);
