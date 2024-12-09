#pragma once

#include <cstdint>

class GF {
public:
  GF(uint16_t modulo) : modulo(modulo) {}

  uint8_t add(uint8_t x, uint8_t y) { return x ^ y; }

  uint8_t multiply(uint8_t x, uint8_t y) {
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
    return result;
  }

private:
  uint16_t modulo;
};
