#include "LSX.h"
#include "Util.h"

#include <gtest/gtest.h>

TEST(LSX, S_transform) {
  LSXForward::precalc();
  LSXForward lsx;

  auto block1 = blockFrom(0xffeeddccbbaa9988, 0x1122334455667700);
  auto block2 = blockFrom(0xb66cd8887d38e8d7, 0x7765aeea0c9a7efc);
  auto block3 = blockFrom(0x559d8dd7bd06cbfe, 0x7e7b262523280d39);
  auto block4 = blockFrom(0x0c3322fed531e463, 0x0d80ef5c5a81c50b);
  auto block5 = blockFrom(0x23ae65633f842d29, 0xc5df529c13f5acda);

  ASSERT_EQ(lsx.substitutionForward(block1), block2);
  ASSERT_EQ(lsx.substitutionForward(block2), block3);
  ASSERT_EQ(lsx.substitutionForward(block3), block4);
  ASSERT_EQ(lsx.substitutionForward(block4), block5);
}

TEST(LSX, S_transform_reversible) {
  LSXForward::precalc();
  LSXBackward::precalc();
  LSXForward lsxFwd;
  LSXBackward lsxBwd;

  auto block1 = blockFrom(0xffeeddccbbaa9988, 0x1122334455667700);
  auto block2 = blockFrom(0xb66cd8887d38e8d7, 0x7765aeea0c9a7efc);
  auto block3 = blockFrom(0x559d8dd7bd06cbfe, 0x7e7b262523280d39);
  auto block4 = blockFrom(0x0c3322fed531e463, 0x0d80ef5c5a81c50b);
  auto block5 = blockFrom(0x23ae65633f842d29, 0xc5df529c13f5acda);

  ASSERT_EQ(lsxFwd.substitutionForward(block1), block2);
  ASSERT_EQ(lsxFwd.substitutionForward(block2), block3);
  ASSERT_EQ(lsxFwd.substitutionForward(block3), block4);
  ASSERT_EQ(lsxFwd.substitutionForward(block4), block5);

  ASSERT_EQ(lsxBwd.substitutionBackward(block2), block1);
  ASSERT_EQ(lsxBwd.substitutionBackward(block3), block2);
  ASSERT_EQ(lsxBwd.substitutionBackward(block4), block3);
  ASSERT_EQ(lsxBwd.substitutionBackward(block5), block4);
}

TEST(LSX, R_transform_1) {
  LSXForward::precalc();
  LSXForward lsx;

  auto block1 = blockFrom(0x0000000000000000, 0x0000000000000100);
  auto block2 = blockFrom(0x9400000000000000, 0x0000000000000001);
  auto block3 = blockFrom(0xa594000000000000, 0x0000000000000000);
  auto block4 = blockFrom(0x64a5940000000000, 0x0000000000000000);
  auto block5 = blockFrom(0x0d64a59400000000, 0x0000000000000000);

  ASSERT_EQ(lsx.linearForward(1, block1), block2);
  ASSERT_EQ(lsx.linearForward(1, block2), block3);
  ASSERT_EQ(lsx.linearForward(1, block3), block4);
  ASSERT_EQ(lsx.linearForward(1, block4), block5);
}

TEST(LSX, R_transform_2) {
  LSXForward::precalc();
  LSXForward lsx;

  auto block1 = blockFrom(0x0000000000000000, 0x0000000000000001);
  auto block2 = blockFrom(0x0100000000000000, 0x0000000000000000);
  auto block3 = blockFrom(0x9401000000000000, 0x0000000000000000);
  auto block4 = blockFrom(0x8494010000000000, 0x0000000000000000);
  auto block5 = blockFrom(0xDD84940100000000, 0x0000000000000000);

  ASSERT_EQ(lsx.linearForward(1, block1), block2);
  ASSERT_EQ(lsx.linearForward(1, block2), block3);
  ASSERT_EQ(lsx.linearForward(1, block3), block4);
  ASSERT_EQ(lsx.linearForward(1, block4), block5);
}

TEST(LSX, R_transform_reversible) {
  LSXForward::precalc();
  LSXBackward::precalc();

  LSXForward lsxFwd;
  LSXBackward lsxBwd;

  auto block1 = blockFrom(0x0000000000000000, 0x0000000000000001);
  auto block2 = blockFrom(0x0100000000000000, 0x0000000000000000);

  ASSERT_EQ(lsxFwd.linearForward(1, block1), block2);
  ASSERT_EQ(lsxBwd.linearBackward(1, block2), block1);
}

TEST(LSX, L_transform) {
  LSXForward::precalc();
  LSXForward lsx;

  auto block1 = blockFrom(0x64a5940000000000, 0x0000000000000000);
  auto block2 = blockFrom(0xd456584dd0e3e84c, 0xc3166e4b7fa2890d);
  auto block3 = blockFrom(0x79d26221b87b584c, 0xd42fbc4ffea5de9a);
  auto block4 = blockFrom(0x0e93691a0cfc6040, 0x8b7b68f66b513c13);
  auto block5 = blockFrom(0xe6a8094fee0aa204, 0xfd97bcb0b44b8580);

  ASSERT_EQ(lsx.linearForward(16, block1), block2);
  ASSERT_EQ(lsx.linearForward(16, block2), block3);
  ASSERT_EQ(lsx.linearForward(16, block3), block4);
  ASSERT_EQ(lsx.linearForward(16, block4), block5);
}

TEST(LSX, L_transform_reversible) {
  LSXForward::precalc();
  LSXBackward::precalc();
  LSXForward lsxFwd;
  LSXBackward lsxBwd;

  auto block1 = blockFrom(0x64a5940000000000, 0x0000000000000000);
  auto block2 = blockFrom(0xd456584dd0e3e84c, 0xc3166e4b7fa2890d);
  auto block3 = blockFrom(0x79d26221b87b584c, 0xd42fbc4ffea5de9a);
  auto block4 = blockFrom(0x0e93691a0cfc6040, 0x8b7b68f66b513c13);
  auto block5 = blockFrom(0xe6a8094fee0aa204, 0xfd97bcb0b44b8580);

  ASSERT_EQ(lsxFwd.linearForward(16, block1), block2);
  ASSERT_EQ(lsxFwd.linearForward(16, block2), block3);
  ASSERT_EQ(lsxFwd.linearForward(16, block3), block4);
  ASSERT_EQ(lsxFwd.linearForward(16, block4), block5);

  ASSERT_EQ(lsxBwd.linearBackward(16, block2), block1);
  ASSERT_EQ(lsxBwd.linearBackward(16, block3), block2);
  ASSERT_EQ(lsxBwd.linearBackward(16, block4), block3);
  ASSERT_EQ(lsxBwd.linearBackward(16, block5), block4);
}
