#include "LSX.h"
#include "GF.h"
#include "Types.h"

Block LSXForward::operator()(Block key, Block input) {
  auto block = xorForward(key, input);
  block = substitutionForward(block);
  return linearForward(16, block);
}

Block LSXForward::linearForward(int rounds, Block input) {
  GF gf(0b111000011);

  auto bytes = toByteArray(input);
  while (rounds--) {
    BlockByteArray result;

    result.data[0] = gf.multiply(1, bytes.data[0]);
    result.data[1] = gf.multiply(148, bytes.data[1]);
    result.data[2] = gf.multiply(32, bytes.data[2]);
    result.data[3] = gf.multiply(133, bytes.data[3]);
    result.data[4] = gf.multiply(16, bytes.data[4]);
    result.data[5] = gf.multiply(194, bytes.data[5]);
    result.data[6] = gf.multiply(192, bytes.data[6]);
    result.data[7] = gf.multiply(1, bytes.data[7]);
    result.data[8] = gf.multiply(251, bytes.data[8]);
    result.data[9] = gf.multiply(1, bytes.data[9]);
    result.data[10] = gf.multiply(192, bytes.data[10]);
    result.data[11] = gf.multiply(194, bytes.data[11]);
    result.data[12] = gf.multiply(16, bytes.data[12]);
    result.data[13] = gf.multiply(133, bytes.data[13]);
    result.data[14] = gf.multiply(32, bytes.data[14]);
    result.data[15] = gf.multiply(148, bytes.data[15]);

    uint8_t transformed = 0;
    for (auto byte : result.data) {
      transformed = gf.add(transformed, byte);
    }

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
