#include "Encryptor.h"
#include "Types.h"
#include "Util.h"

#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>

volatile sig_atomic_t quitok = false;
void handle_break(int a) {
  if (a == SIGINT) {
    quitok = true;
  }
}

int main() {
  struct sigaction sigbreak;
  sigbreak.sa_handler = &handle_break;
  sigemptyset(&sigbreak.sa_mask);
  sigbreak.sa_flags = 0;
  sigaction(SIGINT, &sigbreak, NULL);

  auto encryptor = Encryptor::withKey(KeyType{123, 456});

  uint64_t processed = 0;
  std::chrono::nanoseconds estimated_time = std::chrono::nanoseconds::zero();

  Block block = 0;
  while (!quitok && estimated_time.count() < 5e9) {
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