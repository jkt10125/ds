#ifndef FAST_IO_HPP
#define FAST_IO_HPP

#include "common.hpp"
#include <cstdio>
#include <array>
#include <cstring>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <print>

#if defined(__AVX2__)
#include <immintrin.h>
#endif

namespace internal {
  struct endline {} endl;
}

template <u32 buffer_size>
struct fast_input {
  FILE *file;
  char *buf;
  char *cur;
  char *end;
  u64  mmap_size;

  explicit fast_input(FILE *file = stdin) : file(file) {
    struct stat st;
    i32 fd = fileno(file);
    fstat(fd, &st);
    mmap_size = st.st_size;
    buf = static_cast<char*>(mmap(nullptr, mmap_size, PROT_READ, MAP_PRIVATE, fd, 0));
    cur = buf;
    end = buf + mmap_size;
  }

  ~fast_input() {
    munmap(buf, mmap_size);
  }

  void skip_spaces() {
    while (cur < end && *cur < 33) {
      ++cur;
    }
  }

  #if defined(__AVX2__)
  static constexpr u64 e16 = 10'000'000'000'000'000ull;
  static inline const __m128i zero_128 = _mm_set1_epi8(0x30);
  static inline const __m256i zero_256 = _mm256_set1_epi8(0x30);
  static inline const __m128i w1_128 = _mm_set1_epi16(0x010a);
  static inline const __m256i w1_256 = _mm256_set1_epi16(0x010a);
  static inline const __m128i w2_128 = _mm_set1_epi32(0x00010064);
  static inline const __m256i w2_256 = _mm256_set1_epi32(0x00010064);
  static inline const __m128i w3_128 = _mm_set_epi32(1, 10000, 1, 10000);
  static inline const __m256i w3_256 = _mm256_set_epi32(1, 10000, 1, 10000, 1, 10000, 1, 10000);

  static constexpr auto lut = [] {
    std::array<std::array<char, 16>, 17> res;
    for (int i = 0; i <= 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        res[i][j] = (i + j < 16) ? 0x80 : i + j - 16;
      }
    }
    return res;
  }();

  static inline u64 parse_w16(__m128i chunk) {
    __m128i t1 = _mm_maddubs_epi16(chunk, w1_128);
    __m128i t2 = _mm_madd_epi16(t1, w2_128);
    __m128i prod = _mm_mul_epu32(t2, w3_128);
    __m128i odd = _mm_srli_epi64(t2, 32);
    __m128i t3 = _mm_add_epi64(prod, odd);
    u64 r0 = _mm_cvtsi128_si64(t3);
    u64 r1 = _mm_extract_epi64(t3, 1);
    return r0 * 100'000'000 + r1;
  }

  static inline u128 parse_w32(__m256i chunk) {
    __m256i t1 = _mm256_maddubs_epi16(chunk, w1_256);
    __m256i t2 = _mm256_madd_epi16(t1, w2_256);
    __m256i prod = _mm256_mul_epu32(t2, w3_256);
    __m256i odd = _mm256_srli_epi64(t2, 32);
    __m256i t3 = _mm256_add_epi64(prod, odd);
    __m128i r0 = _mm256_castsi256_si128(t3);
    __m128i r1 = _mm256_extracti128_si256(t3, 1);
    u64 s0 = _mm_cvtsi128_si64(r0) * 100'000'000 + _mm_extract_epi64(r0, 1);
    u64 s1 = _mm_cvtsi128_si64(r1) * 100'000'000 + _mm_extract_epi64(r1, 1);
    return static_cast<u128>(s0) * e16 + s1;
  }

  constexpr inline bool all_digits(u32 v) {
    return !(v & 0xf0f0f0f0);
  }

  constexpr inline bool all_digits(u64 v) {
    return !(v & 0xf0f0f0f0f0f0f0f0ull);
  }
#endif
  
  template <internal::unsigned_integral T>
  T read() {
#if defined(__AVX2__)
    if constexpr (sizeof(T) < 8) {
      // The ASCII representation of each digit is its bottom 4 bits
      T x = *cur++ & 0x0f;
      u64 v;
      memcpy(&v, cur, 8);
      v ^= 0x3030303030303030ull;
      if (all_digits(v)) {
        v = (v * 10 + (v >> 8)) & 0xff00ff00ff00ffull;
        v = (v * 100 + (v >> 16)) & 0xffff0000ffffull;
        v = (v * 10000 + (v >> 32)) & 0xffffffffull;
        x = static_cast<T>(x * 100000000 + v);
        cur += 8;
      }
      // fallback loop
      for (; *cur >= 48; ++cur) {
        x = x * 10 + (*cur & 15);
      }
      return x;
    }
    else if constexpr (sizeof(T) == 8) {
      __m128i raw = _mm_loadu_si128(reinterpret_cast<const __m128i*>(cur));
      __m128i diff = _mm_sub_epi8(raw, zero_128);
      u32 mask = _mm_movemask_epi8(diff);
      if (!mask) {
        u64 x = parse_w16(diff);
        cur += 16;
        for (; *cur >= 48; ++cur) {
          x = x * 10 + (*cur & 15);
        }
        return x;
      }
      else {
        i32 len = __builtin_ctz(mask);
        cur += len;
        diff = _mm_shuffle_epi8(diff, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&lut[len])));
        return parse_w16(diff);
      }
    }
    else {
      __m256i raw = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));
      __m256i diff = _mm256_sub_epi8(raw, zero_256);
      u32 mask = _mm256_movemask_epi8(diff);
      if (!mask) {
        u128 x = parse_w32(diff);
        cur += 32;
        for (; *cur >= 48; ++cur) {
          x = x * 10 + (*cur & 15);
        }
        return x;
      }
      else {
        i32 len = __builtin_ctz(mask);
        cur += len;
        if (len <= 16) {
          __m128i low = _mm256_castsi256_si128(diff);
          low = _mm_shuffle_epi8(low, _mm_loadu_si128(reinterpret_cast<const __m128i*>(&lut[len])));
          return parse_w16(low);
        }
        else {
          alignas(32) char aux[64]{};
          _mm256_storeu_si256(reinterpret_cast<__m256i*>(aux + 32 - len), diff);
          diff = _mm256_load_si256(reinterpret_cast<const __m256i*>(aux));
          return parse_w32(diff);
        }
      }
    }
#else
    T x = 0;
    for (; *cur >= 48; ++cur) {
      x = x * 10 + (*cur & 15);
    }
    return x;
#endif
  }

  template <internal::signed_integral T>
  T read() {
    using U = internal::make_unsigned_t<T>;

    bool neg = *cur == '-';
    cur += neg;
    U v = read<U>();
    return static_cast<T>(neg ? -v : v);
  }

  template <typename T>
  requires(internal::same_as<T, char>)
  T read() {
    return *cur++;
  }

  template <typename T>
  requires(internal::same_as<T, bool>)
  T read() {
    return *cur++ != '0';
  }

  template <typename T>
  requires(internal::same_as<T, std::string>)
  T read() {
    char* first = cur;
    while (*cur > 32) ++cur;
    return std::string(first, cur);
  }

  template <typename T>
  requires(!std::is_pointer_v<T>)
  fast_input& operator>>(T& x) {
    skip_spaces();
    x = read<T>();
    return *this;
  }

  fast_input& operator>>(char *x) {
    skip_spaces();
    char* first = cur;
    while (*cur > 32) ++cur;
    std::memcpy(x, first, cur - first);
    x[cur - first] = '\0';
    return *this;
  }
};

template <u32 buffer_size>
struct fast_output {
  FILE *file;
  char *buf;
  char *cur;
  char *end;

  explicit fast_output(FILE *file = stdout) : file(file) {
    buf = new char[buffer_size];
    cur = buf;
    end = buf + buffer_size;
  }

  // Flushes the buffer to the file if there is
  // less than N bytes of space left in the buffer.
  template <u32 N = buffer_size>
  void flush() {
    if (end - cur < N) {
      fwrite(buf, 1, cur - buf, file);
      cur = buf;
    }
  }

  ~fast_output() {
    flush();
    delete[] buf;
  }

  static constexpr auto lut = [] {
    std::array<std::array<char, 4>, 10000> a, b;
    
    for (int i = 0; i < 10000; ++i) {
      b[i][0] = '0' + i / 1000;
      b[i][1] = '0' + i / 100 % 10;
      b[i][2] = '0' + i / 10 % 10;
      b[i][3] = '0' + i % 10;

      int j = 0;
      if (i >= 1000) a[i][j++] = b[i][0];
      if (i >= 100) a[i][j++] = b[i][1];
      if (i >= 10) a[i][j++] = b[i][2];
      a[i][j] = b[i][3];
    }

    return std::make_pair(a, b);
  }();

  static constexpr u64 e16 = 10'000'000'000'000'000ull;
  static constexpr u64 e19 = e16 * 1000;
  static constexpr u128 e38 = static_cast<u128>(e19) * e19;

  template <u8 N, typename T>
  void print(T x) {
    if constexpr (N == 0) {
      memcpy(cur, &lut.first[x], 4);
      cur += 1 + (x > 9) + (x > 99) + (x > 999);
    }
    else {
      print<N - 1>(x / 10000);
      memcpy(cur, &lut.second[x % 10000], 4);
      cur += 4;
    }
  }

  // Prints the full number with leading zeros, up to 4(N + 1) digits.
  template <u8 N, typename T>
  void print_full(T x) {
    if constexpr (N) {
      print_full<N - 1>(x / 10000);
    }
    memcpy(cur, &lut.second[x % 10000], 4);
    cur += 4;
  }

  // Prints a 19-digit number with leading zeros.
  // useful for printing higher order numbers
  void print_w19(u64 x) {
    u16 high = x / e16;
    u64 low = x - static_cast<u64>(high) * e16;
    memcpy(cur, &lut.second[high][1], 3);
    cur += 3;
    print_full<3>(low);
  }

  // Prints a 38-digit number with leading zeros.
  // assumes that x < 1e38
  void print_w38(u128 x) {
    u64 high = static_cast<u64>(x / e19);
    u64 low = static_cast<u64>(x - static_cast<u128>(high) * e19);
    print_w19(high);
    print_w19(low);
  }

  template <typename T>
  void write(T x) {
    if constexpr (sizeof(T) < 8) {
      if (x > 9999'9999) {
        print<2>(x);
      }
      else if (x > 9999) {
        print<1>(x);
      }
      else {
        print<0>(x);
      }
    }
    else if constexpr (sizeof(T) == 8) {
      if (x > 9999'9999'9999'9999ull) {
        print<4>(x);
      }
      else if (x > 9999'9999'9999ull) {
        print<3>(x);
      }
      else if (x > 9999'9999ull) {
        print<2>(x);
      }
      // here x < 1e8, so we cast it to u32
      // and use the 32-bit print function
      else if (x > 9999ull) {
        print<1>(static_cast<u32>(x));
      }
      else {
        print<0>(static_cast<u32>(x));
      }
    }
    else {
      if (x < e19) {
        write(static_cast<u64>(x));
      }
      else if (x < e38) {
        auto high = static_cast<u64>(x / e19);
        auto low = static_cast<u64>(x - static_cast<u128>(high) * e19);
        write(high);
        print_w19(low);
      }
      else {
        auto high = static_cast<u32>(x / e38);
        x -= static_cast<u128>(high) * e38;
        write(high);
        print_w38(x);
      }
    }
  }

  template <internal::unsigned_integral T>
  fast_output& operator<<(T x) {
    flush<std::numeric_limits<T>::digits10 + 1>();
    write(x);
    return *this;
  }

  template <internal::signed_integral T>
  fast_output& operator<<(T x) {
    using U = internal::make_unsigned_t<T>;

    flush<std::numeric_limits<T>::digits10 + 2>();
    *cur = '-';
    cur += (x < 0);
    write(x < 0 ? -static_cast<U>(x) : static_cast<U>(x));
    return *this;
  }

  fast_output& operator<<(char x) {
    flush<1>();
    *cur++ = x;
    return *this;
  }

  fast_output& operator<<(bool x) {
    return *this << (x ? '1' : '0');
  }

  fast_output& operator<<(const char* s) {
    u32 len = strlen(s);
    if (len > buffer_size) [[unlikely]] {
      flush();
      do {
        fwrite(s, 1, buffer_size, file);
        s += buffer_size;
        len -= buffer_size;
      } while (len > buffer_size);
    }
    if (end - cur < len) [[unlikely]] flush();
    memcpy(cur, s, len);
    cur += len;
    return *this;
  }

  fast_output& operator<<(char* s) {
    return *this << const_cast<const char*>(s);
  }

  fast_output& operator<<(const std::string& s) {
    return *this << s.c_str();
  }

  fast_output& operator<<(internal::endline) {
    flush<1>();
    *cur++ = '\n';
    flush();
    return *this;
  }
};

template <u32 ibuf_size = 1 << 20, u32 obuf_size = 1 << 20>
struct fast_io {
  fast_input<ibuf_size>  *in;
  fast_output<obuf_size> *out;

  fast_io(FILE *in_file = stdin, FILE *out_file = stdout) {
    in = new fast_input<ibuf_size>(in_file);
    out = new fast_output<obuf_size>(out_file);
  }

  ~fast_io() {
    delete out;
    delete in;
  }

  fast_io(const fast_io&) = delete;
  fast_io& operator=(const fast_io&) = delete;

  void flush() {
    out->flush();
  }

  fast_io& operator>>(auto& x) {
    *in >> x;
    return *this;
  }

  fast_io& operator<<(auto x) {
    *out << x;
    return *this;
  }

  fast_io& operator<<(internal::endline) {
    *out << internal::endl;
    return *this;
  }
};

#endif  // FAST_IO_HPP
