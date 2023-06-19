/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

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

#include "launcher.h"
#include "randomx.h"
#include <iostream>
#include <vector>
#include "dataset.hpp"
#include "blake2/endian.h"
#include "common.hpp"

using HashType = uint64_t[RANDOMX_HASH_SIZE / sizeof(uint64_t)];

const uint8_t blockTemplate_[] = {
  0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
  0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
  0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
  0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
};

void mine(randomx_vm* vm, HashType &result_hash) {
  uint8_t blockTemplate[sizeof(blockTemplate_)];
  memcpy(blockTemplate, blockTemplate_, sizeof(blockTemplate));

  randomx_calculate_hash(vm, blockTemplate, sizeof(blockTemplate), &result_hash);
}

int main() {
  test_randomx();

  return 0;
}

void test_randomx() {
  std::ios_base::Init init;
  std::cout << "randomX: start" << std::endl;

  // RANDOMX_FLAG_DEFAULT must be set, because others flags could use features like large pages
  randomx_cache *cache = randomx_alloc_cache(RANDOMX_FLAG_DEFAULT);
  if (cache == nullptr) {
    std::cout << "randomX: can't allocate a cache" << std::endl;
  }

  std::cout << "randomX: cache created " << cache << std::endl;

  const char seed[4] = {0, 0, 1, 0};
  randomx_init_cache(cache, &seed, sizeof(seed));
  std::cout << "randomX: cache " << cache << " " << cache->isInitialized() << std::endl;

  randomx_dataset *dataset = nullptr;
  randomx_flags flags = RANDOMX_FLAG_DEFAULT;
  randomx_vm *vm = randomx_create_vm(flags, cache, dataset);

  uint64_t hash[RANDOMX_HASH_SIZE / sizeof(uint64_t)];
  mine(vm, hash);

  std::cout << "randomX: vm created " << vm << std::endl;

  randomx_destroy_vm(vm);
  randomx_release_cache(cache);

  std::cout << "Calculated result: ";
  for (int i = 0; i < sizeof(HashType) / sizeof (uint64_t); ++i) {
    std::cout << std::hex << hash[i] << " ";
  }
  std::cout << std::endl;
}
