#include <cstring>
#include <cmath>
#include "number.h"

int2023_t from_int(int32_t i) { // NOLINT(*-no-recursion)
  int2023_t result;
  uint32_t value = std::abs(i);

  for (int32_t j = 0; value > 0; ++j) {
    result.value[j] = value & kByteMask;
    value >>= kByteSize;
  }
  return i > 0 ? result : ~result;
}

int2023_t from_string(const char* number) {
  int2023_t result;
  bool is_negative = number[0] == '-';
  size_t size = strlen(number);
  auto buf = new uint8_t[size - is_negative]();

  for (int i = is_negative; i < size; ++i) {
    buf[i - is_negative] = number[i] - '0';
  }

  for (uint8_t& i : result.value) {
    int tmp = 0;
    for (int j = 0; j < size - is_negative; ++j) {
      tmp = (tmp * 10) + buf[j];
      if (tmp >= 256) {
        buf[j] = tmp / 256;
        tmp %= 256;
      } else {
        buf[j] = 0;
      }
    }
    i = tmp;
  }

  delete[] buf;
  return is_negative ? ~result : result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = lhs;
  result += rhs;
  return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = lhs;
  result += (~rhs);
  return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;

  bool is_lhs_negative = IsNegative(lhs);
  bool is_rhs_negative = IsNegative(rhs);

  int2023_t lhs_copy = is_lhs_negative ? ~lhs : lhs;
  int2023_t rhs_copy = is_rhs_negative ? ~rhs : rhs;

  for (unsigned char i : rhs_copy.value) {
    for (int j = 0; j < kByteSize; ++j) {
      if (i & (1 << j)) {
        result += lhs_copy;
      }
      lhs_copy <<= 1;
    }
  }

  return (is_lhs_negative ^ is_rhs_negative) ? ~result : result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;
  int2023_t buf;

  bool is_lhs_negative = IsNegative(lhs);
  bool is_rhs_negative = IsNegative(rhs);

  int2023_t lhs_copy = is_lhs_negative ? ~lhs : lhs;
  int2023_t rhs_copy = is_rhs_negative ? ~rhs : rhs;

  for (int32_t j = int2023_t::kMaxSize - 1; j >= 0; --j) {
    result <<= 8;
    buf <<= 8;
    buf.value[0] += lhs_copy.value[j];

    while (buf >= rhs_copy) {
      buf = buf - rhs_copy;
      result.value[0] += 1;
    }
  }

  return (is_lhs_negative ^ is_rhs_negative) ? ~result : result;
}
bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i < int2023_t::kMaxSize; ++i) {
    if (lhs.value[i] != rhs.value[i]) return false;
  }
  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  bool buf[kByteSize];
  bool prefix_zero = false;

  for (int i = int2023_t::kMaxSize - 1; i >= 0; --i) {
    uint8_t byte = value.value[i];

    for (int j = kByteSize - 1; j >= 0; --j) {
      buf[j] = byte % 2;
      if (buf[j]) prefix_zero = true;
      byte /= 2;
    }

    if (prefix_zero) {
      for (const bool& j : buf) {
        stream << j;
      }
      stream << " ";
    }
  }

  return stream;
}

int2023_t operator~(const int2023_t& i) { // NOLINT(*-no-recursion)
  int2023_t result;

  for (int32_t idx = 0; idx < int2023_t::kMaxSize; ++idx) {
    result.value[idx] = ~i.value[idx];
  }

  return result + from_int(1);
}

void operator+=(int2023_t& lhs, const int2023_t& rhs) {
  uint16_t cf = 0;

  for (int i = 0; i < int2023_t::kMaxSize; ++i) {
    cf = static_cast<uint16_t>(lhs.value[i]) + static_cast<uint16_t>(rhs.value[i]) + cf;
    lhs.value[i] = cf & kByteMask;
    cf >>= kByteSize;
  }
}

int2023_t operator<<(int2023_t& lhs, const uint8_t& rhs) {
  int2023_t result = lhs;
  result <<= rhs;
  return result;
}

void operator<<=(int2023_t& lhs, const uint8_t& rhs) {
  uint16_t buf;

  for (unsigned char& i : lhs.value) {
    uint16_t tmp = i << rhs;
    i = (tmp & kByteMask) + buf;
    buf = tmp >> kByteSize;
  }
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = int2023_t::kMaxSize - 1; i >= 0; --i) {
    if (lhs.value[i] > rhs.value[i]) {
      return true;
    } else if (lhs.value[i] < rhs.value[i]) {
      return false;
    }
  }
  return true;
}

bool IsNegative(const int2023_t& i) {
  return i.value[252] & kSignMask;
}