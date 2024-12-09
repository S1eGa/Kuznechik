#pragma once

#include "Types.h"
#include <utility>

template <typename F> class FeistelRound {
public:
  std::pair<Block, Block> operator()(Block key, std::pair<Block, Block> input) {
    return {F{}(key, input.first) ^ input.second, input.first};
  }
};