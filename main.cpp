#include "Encryptor.h"
#include "Types.h"
#include "Util.h"

#include <chrono>
#include <cstdint>
#include <iostream>

int main() {
  auto encryptor = Encryptor::withKey(KeyType{123, 456});

  uint64_t processed = 0;
  std::chrono::nanoseconds estimated_time = std::chrono::nanoseconds::zero();

  Block block = 0;
  while (estimated_time.count() < 5e9) {
    estimated_time += measure<std::chrono::nanoseconds>::duration(
        &Encryptor::encrypt, &encryptor, block);
    processed += sizeof(block);
    block++;
  }

  std::cout << "Encryption bandwidth is about "
            << static_cast<double>(processed) / (1 << 20) /
                   (estimated_time.count() / 1e9)
            << " megabytes/sec" << std::endl;
}