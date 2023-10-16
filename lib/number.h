#pragma once
#include <cinttypes>
#include <iostream>
#include <limits>

static const uint8_t kByteMask = 0b11111111;
static const uint8_t kSignMask = 0b10000000;
static const uint8_t kByteSize = 8;

struct int2023_t {
  static const size_t kMaxSize = 253;
  uint8_t value[kMaxSize] = {0};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator~(const int2023_t& i);

void operator+=(int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

int2023_t operator<<(int2023_t& lhs, const uint8_t& rhs);

void operator<<=(int2023_t& lhs, const uint8_t& rhs);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

bool IsNegative(const int2023_t& i);