#pragma once

#include "Types.h"

class LSXForward {
public:
  Block operator()(Block, Block);

  Block linearForward(int rounds, Block);

  Block substitutionForward(Block);
  Block xorForward(Block, Block);

  Block substitutionLinearForward(int rounds, Block);

  static void precalc();

private:
  inline static Block table[16][16][256];
};

class LSXBackward {
public:
  Block operator()(Block, Block);

  Block linearBackward(int rounds, Block);

  Block substitutionBackward(Block);
  Block xorBackward(Block, Block);

  static void precalc();

private:
  inline static Block table[16][16][256];
};
