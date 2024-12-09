#pragma once

#include "LSX.h"
#include "Types.h"
class Encryptor {
private:
  LSXForward lsx;

  Encryptor(KeyType key);

public:
  static Encryptor withKey(KeyType key);

  Block encrypt(Block block);
  Block decrypt(Block block);

// Commented for testing
// private:
  Block itKey[10];
};
