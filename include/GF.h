#pragma once

#include <cstdint>

template <uint16_t modulo> class GF {
public:
  static void precalc() {
    for (int x = 0; x < 256; ++x) {
      for (int y = 0; y < 256; ++y) {

        uint16_t result = 0;
        for (int i = 0; i < 8; ++i) {
          if ((x >> i) & 1) {
            result ^= y << i;
          }
        }

        for (int i = 15; i >= 8; --i) {
          if ((result >> i) & 1) {
            result ^= modulo << (i - 8);
          }
        }
        table[x][y] = result;
      }
    }
  }

  uint8_t add(uint8_t x, uint8_t y) { return x ^ y; }

  uint8_t multiply(uint8_t x, uint8_t y) { return table[x][y]; }

private:
  inline static uint8_t table[256][256];
};
