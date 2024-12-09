#pragma once

#include "Types.h"

class LSXForward {
public:
  Block operator()(Block, Block);

  Block linearForward(int rounds, Block);

  Block substitutionForward(Block);
  Block xorForward(Block, Block);

  static void precalc();
private:
  inline static Block table[16][16][256];
};