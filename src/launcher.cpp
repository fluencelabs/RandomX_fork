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
#include "common.hpp"

int main() {
  test_randomx();

  return 0;
}

void test_randomx() {
  std::cout << "randomX: start" << std::endl;

  // RANDOMX_FLAG_DEFAULT must be set, because others flags could use features like large pages
  randomx_flags flags = RANDOMX_FLAG_DEFAULT;
  randomx_cache *cache = randomx_alloc_cache(flags);
  if (cache == nullptr) {
    std::cout << "randomX: can't allocate a cache" << std::endl;
  }

  std::cout << "randomX: cache created " << cache << std::endl;

  const char seed[] = "RandomX example key";
  randomx_init_cache(cache, &seed, sizeof(seed));
  std::cout << "randomX: cache " << cache << " " << cache->isInitialized() << std::endl;

  randomx_vm *vm = randomx_create_vm(flags, cache, nullptr);

  std::cout << "randomX: vm created " << vm << std::endl;

  char hash[RANDOMX_HASH_SIZE];
  const char input[] = "RandomX example input";
  randomx_calculate_hash(vm, &input, sizeof input, hash);

  randomx_destroy_vm(vm);
  randomx_release_cache(cache);

  for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i) {
    printf("%02x", hash[i] & 0xff);
  }

  printf("\n");
}
