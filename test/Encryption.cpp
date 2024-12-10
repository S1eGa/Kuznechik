#include "Encryptor.h"
#include "Types.h"
#include "Util.h"

#include <gtest/gtest.h>

TEST(Key, Encryption_1) {
  KeyType key = {blockFrom(0x8899aabbccddeeff, 0x0011223344556677),
                 blockFrom(0xfedcba9876543210, 0x0123456789abcdef)};

  auto block1 = blockFrom(0x1122334455667700, 0xffeeddccbbaa9988);
  auto block2 = blockFrom(0x7f679d90bebc2430, 0x5a468d42b9d4edcd);

  auto encryptor = Encryptor::withKey(key);
  ASSERT_EQ(encryptor.encrypt(block1), block2);
}

TEST(Key, Encryption_Decryption) {
  KeyType key = {blockFrom(0x8899aabbccddeeff, 0x0011223344556677),
                 blockFrom(0xfedcba9876543210, 0x0123456789abcdef)};

  auto block1 = blockFrom(0x1122334455667700, 0xffeeddccbbaa9988);
  auto block2 = blockFrom(0x7f679d90bebc2430, 0x5a468d42b9d4edcd);

  auto encryptor = Encryptor::withKey(key);
  ASSERT_EQ(encryptor.encrypt(block1), block2);
  ASSERT_EQ(encryptor.decrypt(block2), block1);
}
