#include "GF.h"
#include "LSX.h"
#include "Types.h"
#include <climits>
#include <cstring>

const uint8_t Pi[256] = {
    252, 238, 221, 17,  207, 110, 49,  22,  251, 196, 250, 218, 35,  197, 4,
    77,  233, 119, 240, 219, 147, 46,  153, 186, 23,  54,  241, 187, 20,  205,
    95,  193, 249, 24,  101, 90,  226, 92,  239, 33,  129, 28,  60,  66,  139,
    1,   142, 79,  5,   132, 2,   174, 227, 106, 143, 160, 6,   11,  237, 152,
    127, 212, 211, 31,  235, 52,  44,  81,  234, 200, 72,  171, 242, 42,  104,
    162, 253, 58,  206, 204, 181, 112, 14,  86,  8,   12,  118, 18,  191, 114,
    19,  71,  156, 183, 93,  135, 21,  161, 150, 41,  16,  123, 154, 199, 243,
    145, 120, 111, 157, 158, 178, 177, 50,  117, 25,  61,  255, 53,  138, 126,
    109, 84,  198, 128, 195, 189, 13,  87,  223, 245, 36,  169, 62,  168, 67,
    201, 215, 121, 214, 246, 124, 34,  185, 3,   224, 15,  236, 222, 122, 148,
    176, 188, 220, 232, 40,  80,  78,  51,  10,  74,  167, 151, 96,  115, 30,
    0,   98,  68,  26,  184, 56,  130, 100, 159, 38,  65,  173, 69,  70,  146,
    39,  94,  85,  47,  140, 163, 165, 125, 105, 213, 149, 59,  7,   88,  179,
    64,  134, 172, 29,  247, 48,  55,  107, 228, 136, 217, 231, 137, 225, 27,
    131, 73,  76,  63,  248, 254, 141, 83,  170, 144, 202, 216, 133, 97,  32,
    113, 103, 164, 45,  43,  9,   91,  203, 155, 37,  208, 190, 229, 108, 82,
    89,  166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57,  75,  99,
    182};

Block LSXForward::operator()(Block key, Block input) {
  auto block = xorForward(key, input);
  return substitutionLinearForward(16, block);
}

Block LSXForward::substitutionLinearForward(int rounds, Block input) {
  auto bytes = toByteArray(input);
  Block result = (rounds == 16 ? 0 : input >> (rounds * CHAR_BIT));

  for (int i = 0; i < 16; ++i) {
    result ^= table[i][rounds - 1][Pi[bytes.data[i]]];
  }

  return result;
}

Block LSXForward::linearForward(int rounds, Block input) {
  auto bytes = toByteArray(input);
  Block result = (rounds == 16 ? 0 : input >> (rounds * CHAR_BIT));

  for (int i = 0; i < 16; ++i) {
    result ^= table[i][rounds - 1][bytes.data[i]];
  }

  return result;
}

Block LSXForward::substitutionForward(Block input) {
  auto bytes = toByteArray(input);
  for (auto &byte : bytes.data) {
    byte = Pi[byte];
  }
  return fromByteArray(std::move(bytes));
}

Block LSXForward::xorForward(Block key, Block input) { return input ^ key; }

void LSXForward::precalcLTable() {
  GF<0b111000011> gf;

  memset(L, 0, 256);

  // Initialize single transformation
  // l[16] = l[0] * m[0] + l[1] * m[1] + ... + l[15] * m[15]
  // l[15] = l[14]
  // l[14] = l[13]
  // ...
  // l[1] = l[0]

  L[15][15] = 148;
  L[14][15] = 32;
  L[13][15] = 133;
  L[12][15] = 16;
  L[11][15] = 194;
  L[10][15] = 192;
  L[9][15] = 1;
  L[8][15] = 251;
  L[7][15] = 1;
  L[6][15] = 192;
  L[5][15] = 194;
  L[4][15] = 16;
  L[3][15] = 133;
  L[2][15] = 32;
  L[1][15] = 148;
  L[0][15] = 1;

  L[15][14] = 1;
  L[14][13] = 1;
  L[13][12] = 1;
  L[12][11] = 1;
  L[11][10] = 1;
  L[10][9] = 1;
  L[9][8] = 1;
  L[8][7] = 1;
  L[7][6] = 1;
  L[6][5] = 1;
  L[5][4] = 1;
  L[4][3] = 1;
  L[3][2] = 1;
  L[2][1] = 1;
  L[1][0] = 1;

  // Do matrix exponentiation
  uint8_t newLTransformationMatrix[16][16];
  // NB: We may do exponentiation faster, but as we are
  // inside precalc, it is not required
  for (int p = 0; p < 4; ++p) {
    memset(newLTransformationMatrix, 0, 256);
    for (int i = 0; i < 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        for (int k = 0; k < 16; ++k) {
          newLTransformationMatrix[i][j] ^= gf.multiply(L[i][k], L[k][j]);
        }
      }
    }
    memcpy(L, newLTransformationMatrix, 256);
  }
}

void LSXForward::precalc() {
  GF<0b111000011>::precalc();
  GF<0b111000011> gf;

  precalcLTable();

  for (int i = 0; i < 16; ++i) {
    for (int m = 0; m < 256; ++m) {
      Block block = 0;
      for (int j = 0; j < 16; ++j) {
        Block add = gf.multiply(m, L[i][j]);
        block = (add << 120) | (block >> 8);
        table[i][j][m] = block;
      }
    }
  }
}
