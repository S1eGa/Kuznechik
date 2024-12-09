#include "LSX.h"
#include "GF.h"
#include "Types.h"

Block LSXForward::operator()(Block key, Block input) {
  auto block = xorForward(key, input);
  block = substitutionForward(block);
  return linearForward(16, block);
}

Block LSXForward::linearForward(int rounds, Block input) {
  GF<0b111000011> gf;

  auto bytes = toByteArray(input);
  while (rounds--) {
    uint8_t transformed = 0;

    transformed = gf.add(transformed, gf.multiply(1, bytes.data[0]));
    transformed = gf.add(transformed, gf.multiply(148, bytes.data[1]));
    transformed = gf.add(transformed, gf.multiply(32, bytes.data[2]));
    transformed = gf.add(transformed, gf.multiply(133, bytes.data[3]));
    transformed = gf.add(transformed, gf.multiply(16, bytes.data[4]));
    transformed = gf.add(transformed, gf.multiply(194, bytes.data[5]));
    transformed = gf.add(transformed, gf.multiply(192, bytes.data[6]));
    transformed = gf.add(transformed, gf.multiply(1, bytes.data[7]));
    transformed = gf.add(transformed, gf.multiply(251, bytes.data[8]));
    transformed = gf.add(transformed, gf.multiply(1, bytes.data[9]));
    transformed = gf.add(transformed, gf.multiply(192, bytes.data[10]));
    transformed = gf.add(transformed, gf.multiply(194, bytes.data[11]));
    transformed = gf.add(transformed, gf.multiply(16, bytes.data[12]));
    transformed = gf.add(transformed, gf.multiply(133, bytes.data[13]));
    transformed = gf.add(transformed, gf.multiply(32, bytes.data[14]));
    transformed = gf.add(transformed, gf.multiply(148, bytes.data[15]));

    for (int i = 1; i < 16; ++i) {
      std::swap(bytes.data[i], bytes.data[i - 1]);
    }
    bytes.data[15] = transformed;
  }

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
