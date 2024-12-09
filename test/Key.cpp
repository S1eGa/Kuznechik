#include "Encryptor.h"
#include "Types.h"
#include "Util.h"

#include <gtest/gtest.h>

TEST(Key, Deployment) {
  KeyType key = {blockFrom(0x8899aabbccddeeff, 0x0011223344556677),
                 blockFrom(0xfedcba9876543210, 0x0123456789abcdef)};

  auto encryptor = Encryptor::withKey(key);
  auto &keys = encryptor.itKey;

  auto block1 = blockFrom(0x8899aabbccddeeff, 0x0011223344556677);
  auto block2 = blockFrom(0xfedcba9876543210, 0x0123456789abcdef);
  auto block3 = blockFrom(0xdb31485315694343, 0x228d6aef8cc78c44);
  auto block4 = blockFrom(0x3d4553d8e9cfec68, 0x15ebadc40a9ffd04);
  auto block5 = blockFrom(0x57646468c44a5e28, 0xd3e59246f429f1ac);
  auto block6 = blockFrom(0xbd079435165c6432, 0xb532e82834da581b);
  auto block7 = blockFrom(0x51e640757e8745de, 0x705727265a0098b1);
  auto block8 = blockFrom(0x5a7925017b9fdd3e, 0xd72a91a22286f984);
  auto block9 = blockFrom(0xbb44e25378c73123, 0xa5f32f73cdb6e517);
  auto block10 = blockFrom(0x72e9dd7416bcf45b, 0x755dbaa88e4a4043);

  ASSERT_EQ(keys[0], block1);
  ASSERT_EQ(keys[1], block2);
  ASSERT_EQ(keys[2], block3);
  ASSERT_EQ(keys[3], block4);
  ASSERT_EQ(keys[4], block5);
  ASSERT_EQ(keys[5], block6);
  ASSERT_EQ(keys[6], block7);
  ASSERT_EQ(keys[7], block8);
  ASSERT_EQ(keys[8], block9);
  ASSERT_EQ(keys[9], block10);
}