#include "Encryptor.h"
#include "Feistel.h"
#include "LSX.h"
#include "Types.h"

Encryptor::Encryptor(KeyType key) {
  LSXForward::precalc();
  LSXBackward::precalc();

  Block itConstants[32];
  for (Block i = 0; i < 32; ++i) {
    itConstants[i] = lsxFwd.linearForward(16, i + 1);
  }

  itKey[0] = key.first;
  itKey[1] = key.second;

  for (int i = 1; i < 5; ++i) {
    std::pair<Block, Block> rawRoundKeys = {itKey[(i - 1) * 2],
                                            itKey[(i - 1) * 2 + 1]};

    for (int j = 0; j < 8; ++j) {
      rawRoundKeys = FeistelRound<LSXForward>{}(itConstants[8 * (i - 1) + j],
                                                rawRoundKeys);
    }

    itKey[2 * i] = rawRoundKeys.first;
    itKey[2 * i + 1] = rawRoundKeys.second;
  }
}

Encryptor Encryptor::withKey(KeyType key) { return Encryptor(key); }

Block Encryptor::encrypt(Block block) {
  Block cblock = block;
  for (int i = 0; i < 9; ++i) {
    cblock = lsxFwd(itKey[i], cblock);
  }
  return lsxFwd.xorForward(itKey[9], cblock);
}

Block Encryptor::decrypt(Block cblock) {
  Block block = cblock;
  for (int i = 9; i > 0; --i) {
    block = lsxBwd(itKey[i], block);
  }
  return lsxBwd.xorBackward(itKey[0], block);
}
