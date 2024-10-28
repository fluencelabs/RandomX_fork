/*
Copyright (c) 2024, Cloudless Labs.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the copyright holder nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../common_flu.hpp"
#include "utility.hpp"
#include <exception>
#include <iostream>

const uint8_t blockTemplate_[] = {
    0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08,
    0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14, 0x5a, 0xc5, 0xfa,
    0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f,
    0x85, 0x3f, 0x00, 0x2b, 0x2e, 0xea, 0x00, 0x00, 0x00, 0x00, 0x77,
    0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf,
    0x0a, 0xca, 0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9,
    0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09};

class Hash {
public:
  Hash() {
    for (int i = 0; i < 4; ++i)
      hash[i] = 0;
  }
  void xorWith(uint64_t update[4]) {
    for (int i = 0; i < 4; ++i)
      hash[i] ^= update[i];
  }
  void print(std::ostream &os) {
    for (int i = 0; i < 4; ++i)
      print(hash[i], os);
    os << std::endl;
  }
  static void print(uint64_t &hash, std::ostream &os) {
    auto h = hash;
    outputHex(std::cout, (char *)&h, sizeof(h));
  }
  uint64_t hash[4];
};


int main(int argc, char **argv) {
  bool softAes, miningMode, verificationMode, help, largePages, jit, secure,
      commit;
  bool ssse3, avx2, autoFlags, noBatch;
  int noncesCount, threadCount, initThreadCount;
  uint64_t threadAffinity;
  int32_t seedValue;

  randomx_vm *vm;
  randomx_cache *cache;
  randomx_flags flags = RANDOMX_FLAG_DEFAULT;

  try {
    uint8_t microCache[RANDOMX_PROGRAM_COUNT * RANDOMX_PROGRAM_ITERATIONS *
                       randomx::CacheLineSize];
    memset(microCache, 42, sizeof(microCache));

    randomx_flags micro_flags = RANDOMX_FLAG_MICRO;
    randomx_cache *cache = randomx_alloc_cache(micro_flags);
    cache = randomx_cache_set_micro_cache(cache, reinterpret_cast<void*>(&microCache[0]),
                                          sizeof(microCache));

    Hash micro_hash;

    uint8_t seed45[32];
    memset(seed45, 45, sizeof(seed45));
    randomx_init_cache(cache, &seed45[0], sizeof(seed45));

    randomx_vm *micro_vm = randomx_create_micro_vm(micro_flags, cache, nullptr);
    randomx_calculate_hash(micro_vm, &blockTemplate_, sizeof(blockTemplate_),
                           &micro_hash.hash[0]);
    randomx_destroy_vm(micro_vm);
    std::cout << "Hash micro: ";
    micro_hash.print(std::cout);

    randomx_release_cache(cache);
  } catch (std::exception &e) {
    std::cout << "ERROR: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
