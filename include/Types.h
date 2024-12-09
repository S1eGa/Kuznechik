#pragma once

#include <bit>
#include <cstdint>
#include <utility>

typedef __uint128_t Block;

struct alignas(__int128_t) BlockByteArray {
  uint8_t data[16];
};

inline BlockByteArray toByteArray(Block input) {
  auto result = std::bit_cast<BlockByteArray>(input);
  return result;
}

inline Block fromByteArray(BlockByteArray input) {
  return std::bit_cast<Block>(input);
}

typedef std::pair<__uint128_t, __uint128_t> KeyType;
