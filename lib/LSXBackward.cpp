#include "GF.h"
#include "LSX.h"
#include <cstring>

const uint8_t InversedPi[256] = {
    165, 45,  50,  143, 14,  48,  56,  192, 84,  230, 158, 57,  85,  126, 82,
    145, 100, 3,   87,  90,  28,  96,  7,   24,  33,  114, 168, 209, 41,  198,
    164, 63,  224, 39,  141, 12,  130, 234, 174, 180, 154, 99,  73,  229, 66,
    228, 21,  183, 200, 6,   112, 157, 65,  117, 25,  201, 170, 252, 77,  191,
    42,  115, 132, 213, 195, 175, 43,  134, 167, 177, 178, 91,  70,  211, 159,
    253, 212, 15,  156, 47,  155, 67,  239, 217, 121, 182, 83,  127, 193, 240,
    35,  231, 37,  94,  181, 30,  162, 223, 166, 254, 172, 34,  249, 226, 74,
    188, 53,  202, 238, 120, 5,   107, 81,  225, 89,  163, 242, 113, 86,  17,
    106, 137, 148, 101, 140, 187, 119, 60,  123, 40,  171, 210, 49,  222, 196,
    95,  204, 207, 118, 44,  184, 216, 46,  54,  219, 105, 179, 20,  149, 190,
    98,  161, 59,  22,  102, 233, 92,  108, 109, 173, 55,  97,  75,  185, 227,
    186, 241, 160, 133, 131, 218, 71,  197, 176, 51,  250, 150, 111, 110, 194,
    246, 80,  255, 93,  169, 142, 23,  27,  151, 125, 236, 88,  247, 31,  251,
    124, 9,   13,  122, 103, 69,  135, 220, 232, 79,  29,  78,  4,   235, 248,
    243, 62,  61,  189, 138, 136, 221, 205, 11,  19,  152, 2,   147, 128, 144,
    208, 36,  52,  203, 237, 244, 206, 153, 16,  68,  64,  146, 58,  1,   38,
    18,  26,  72,  104, 245, 129, 139, 199, 214, 32,  10,  8,   0,   76,  215,
    116};

Block LSXBackward::operator()(Block key, Block input) {
  auto block = xorBackward(key, input);
  block = linearBackward(16, block);
  return substitutionBackward(block);
}

Block LSXBackward::linearBackward(int rounds, Block input) {
  auto bytes = toByteArray(input);
  Block result = (rounds == 16 ? 0 : input << (rounds * CHAR_BIT));

  for (int i = 0; i < 16; ++i) {
    result ^= table[i][16 - rounds][bytes.data[i]];
  }

  return result;
}

Block LSXBackward::substitutionBackward(Block input) {
  auto bytes = toByteArray(input);
  for (auto &byte : bytes.data) {
    byte = InversedPi[byte];
  }
  return fromByteArray(std::move(bytes));
}

Block LSXBackward::xorBackward(Block key, Block input) { return key ^ input; }

void LSXBackward::precalcInversedLTable() {
  GF<0b111000011> gf;

  memset(inversedL, 0, 256);

  // Initialize single transformation
  // l[15] = l[16] * m[0] + l[17] * m[1] + ... + l[31] * m[15]
  // l[16] = l[17]
  // l[17] = l[18]
  // ...
  // l[30] = l[31]

  inversedL[15][0] = 1;
  inversedL[14][0] = 148;
  inversedL[13][0] = 32;
  inversedL[12][0] = 133;
  inversedL[11][0] = 16;
  inversedL[10][0] = 194;
  inversedL[9][0] = 192;
  inversedL[8][0] = 1;
  inversedL[7][0] = 251;
  inversedL[6][0] = 1;
  inversedL[5][0] = 192;
  inversedL[4][0] = 194;
  inversedL[3][0] = 16;
  inversedL[2][0] = 133;
  inversedL[1][0] = 32;
  inversedL[0][0] = 148;

  inversedL[14][15] = 1;
  inversedL[13][14] = 1;
  inversedL[12][13] = 1;
  inversedL[11][12] = 1;
  inversedL[10][11] = 1;
  inversedL[9][10] = 1;
  inversedL[8][9] = 1;
  inversedL[7][8] = 1;
  inversedL[6][7] = 1;
  inversedL[5][6] = 1;
  inversedL[4][5] = 1;
  inversedL[3][4] = 1;
  inversedL[2][3] = 1;
  inversedL[1][2] = 1;
  inversedL[0][1] = 1;

  // Do matrix exponentiation
  uint8_t newInversedLTransformationMatrix[16][16];
  // NB: We may do exponentiation faster, but as we are
  // inside precalc, it is not required
  for (int p = 0; p < 4; ++p) {
    memset(newInversedLTransformationMatrix, 0, 256);
    for (int i = 0; i < 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        for (int k = 0; k < 16; ++k) {
          newInversedLTransformationMatrix[i][j] ^=
              gf.multiply(inversedL[i][k], inversedL[k][j]);
        }
      }
    }
    memcpy(inversedL, newInversedLTransformationMatrix, 256);
  }
}

void LSXBackward::precalc() {
  GF<0b111000011>::precalc();
  GF<0b111000011> gf;

  precalcInversedLTable();

  for (int i = 0; i < 16; ++i) {
    for (int m = 0; m < 256; ++m) {
      Block block = 0;
      for (int j = 15; j >= 0; --j) {
        auto add = gf.multiply(m, inversedL[i][j]);
        block = (block << 8) | add;
        table[i][j][m] = block;
      }
    }
  }
}
