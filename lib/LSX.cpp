#include "LSX.h"
#include "GF.h"
#include "Types.h"
#include <algorithm>

Block LSXForward::operator()(Block key, Block input) {
  auto block = xorForward(key, input);
  block = substitutionForward(block);
  return linearForward(16, block);
}

Block LSXForward::linearForward(int rounds, Block input) {
  GF<0b111000011> gf;

  auto bytes = toByteArray(input);
  uint8_t data[32];

  std::copy(bytes.data, bytes.data + 16, data);
  int pos = 16;

  while (rounds--) {
    uint8_t transformed = 0;

    transformed = gf.add(transformed, gf.multiply(1, data[pos - 16 + 0]));
    transformed = gf.add(transformed, gf.multiply(148, data[pos - 16 + 1]));
    transformed = gf.add(transformed, gf.multiply(32, data[pos - 16 + 2]));
    transformed = gf.add(transformed, gf.multiply(133, data[pos - 16 + 3]));
    transformed = gf.add(transformed, gf.multiply(16, data[pos - 16 + 4]));
    transformed = gf.add(transformed, gf.multiply(194, data[pos - 16 + 5]));
    transformed = gf.add(transformed, gf.multiply(192, data[pos - 16 + 6]));
    transformed = gf.add(transformed, gf.multiply(1, data[pos - 16 + 7]));
    transformed = gf.add(transformed, gf.multiply(251, data[pos - 16 + 8]));
    transformed = gf.add(transformed, gf.multiply(1, data[pos - 16 + 9]));
    transformed = gf.add(transformed, gf.multiply(192, data[pos - 16 + 10]));
    transformed = gf.add(transformed, gf.multiply(194, data[pos - 16 + 11]));
    transformed = gf.add(transformed, gf.multiply(16, data[pos - 16 + 12]));
    transformed = gf.add(transformed, gf.multiply(133, data[pos - 16 + 13]));
    transformed = gf.add(transformed, gf.multiply(32, data[pos - 16 + 14]));
    transformed = gf.add(transformed, gf.multiply(148, data[pos - 16 + 15]));

    data[pos++] = transformed;
  }

  std::copy(data + (pos - 16), data + pos, bytes.data);
  return fromByteArray(std::move(bytes));
}

Block LSXForward::substitutionForward(Block input) {
  auto bytes = toByteArray(input);
  for (auto &byte : bytes.data) {
    byte = Pi[byte];
  }
  return fromByteArray(std::move(bytes));
}

Block LSXForward::xorForward(Block key, Block input) { return input ^ key; }
