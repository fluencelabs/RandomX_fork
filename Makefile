TARGET = randomx
CXX = ./wasi-sdk/bin/clang++
SYSROOT = ./wasi-sdk/share/wasi-sysroot
TARGET_TRIPLE = wasm32-wasi
CFLAGS = -fvisibility=hidden
LDFLAGS = -Wl,--demangle,--allow-undefined
EXPORT_FUNCS = \
	--export=test_randomx

RANDOMX_SRC = \
	src/launcher.cpp\
	src/randomx.cpp\
	src/vm_interpreted_light.cpp\
	src/vm_interpreted.cpp\
	src/virtual_machine.cpp\
	src/dataset.cpp\
	src/argon2_core.c\
	src/aes_hash.cpp\
	src/soft_aes.cpp\
	src/reciprocal.c\
	src/allocator.cpp\
	src/superscalar.cpp\
	src/instructions_portable.cpp\
	src/bytecode_machine.cpp\
	src/argon2_ref.c\
	src/blake2/blake2b.c\
	src/blake2_generator.cpp

RANDOMX_FLAGS = \
	-fno-exceptions

.PHONY: default all clean

default: $(TARGET)
all: default

$(TARGET): $(RANDOMX_SRC)
	$(CXX) -O3 --sysroot=$(SYSROOT) --target=$(TARGET_TRIPLE) $(RANDOMX_FLAGS) $(CFLAGS) $(LDFLAGS) -Wl,$(EXPORT_FUNCS) $^ -o $@.wasm

.PRECIOUS: $(TARGET)

clean:
	-rm -f $(TARGET).wasm
