TOOLCHAIN_PREFIX ?= riscv32-unknown-elf-

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

CC := $(TOOLCHAIN_PREFIX)gcc
LD := $(TOOLCHAIN_PREFIX)ld
CXX := $(TOOLCHAIN_PREFIX)g++
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP := $(TOOLCHAIN_PREFIX)objdump

# Link time optimizations
ifeq ($(lto),yes)
OPTFLAGS+=-flto=auto
endif

UBFLAGS := -fno-strict-aliasing -fno-strict-overflow -fno-delete-null-pointer-checks

CFLAGS := -march=rv32im -mabi=ilp32 -Wl,--gc-sections $(OPTFLAGS) $(UBFLAGS) \
         	-DMICROARCHITECTURE=1 \
         	-DAVOID_NATIVE_UINT128_T=1 \
         	-ffreestanding \
         	-nostartfiles \
         	-nostdlib \
         	-fno-exceptions \
         	-mstrict-align \
         	-mcmodel=medany -static -fvisibility=hidden

CXXFLAGS := -std=c++17 -fno-rtti

default: $(TARGET)
all: default

RANDOMX_OBJS = $(patsubst %.c,%.randomx_c.o,$(patsubst %.cpp,%.randomx_cpp.o,$(RANDOMX_SRC)))
TARGETS=$(RANDOMX_OBJS)

.PHONY: default all clean

all: $(TARGETS) randomx_combined.o

%.randomx_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $(<F)

%.randomx_cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $@ $(<F)

randomx_combined.o: $(RANDOMX_OBJS)
	$(LD) -relocatable $(RANDOMX_OBJS) -o randomx_combined.o

clean:
	rm -f *.ld *.elf *.bin *.tmp link.ld *.o *.a
