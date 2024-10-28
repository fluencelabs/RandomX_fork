TOOLCHAIN_PREFIX ?= riscv32-unknown-elf-

RANDOMX_SRC = \
	vector_freestanding.cpp\
	randomx_flu.cpp\
	string_freestanding.cpp\
	vm_interpreted_micro.cpp\
	vm_interpreted_flu.cpp\
	virtual_machine_flu.cpp\
	superscalar_flu.cpp\
	dataset_flu.cpp\
	stubs.cpp\
	argon2_core.c\
	aes_hash.cpp\
	soft_aes.cpp\
	reciprocal.c\
	allocator_flu.cpp\
	instructions_portable_flu.cpp\
	bytecode_machine_flu.cpp\
	argon2_ref.c\
	blake2/blake2b.c\
	blake2_generator.cpp

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
         	-D_GLIBCXX_INCLUDE_NEXT_C_HEADERS=1 \
         	-ffreestanding \
         	-nostartfiles \
         	-nostdlib \
         	-fno-exceptions \
         	-mstrict-align \
			--sysroot=/opt/riscv/riscv32-unknown-elf \
         	-mcmodel=medany -static -fvisibility=hidden

CXXFLAGS := -std=c++11 -fno-rtti

default: all
# all: default

RANDOMX_OBJS = $(patsubst %.c,%.randomx_c.o,$(patsubst %.cpp,%.randomx_cpp.o,$(RANDOMX_SRC)))
TARGETS=$(RANDOMX_OBJS)

.PHONY: default all clean

all: $(TARGETS) randomx_combined.o librandomxflu.a

%.randomx_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $(<)

%.randomx_cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $@ $(<)

randomx_combined.o: $(RANDOMX_OBJS)
	$(LD) -relocatable $(RANDOMX_OBJS) -o randomx_combined.o
 
librandomxflu.a: $(RANDOMX_OBJS)
	ar rcs librandomxflu.a randomx_combined.o

clean:
	rm -f *.ld *.elf *.bin *.tmp link.ld *.o *.a
