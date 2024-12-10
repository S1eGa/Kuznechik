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
  static void precalcLTable();

  inline static uint8_t L[16][16];
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
  static void precalcInversedLTable();

  inline static uint8_t inversedL[16][16];
  inline static Block table[16][16][256];
};
