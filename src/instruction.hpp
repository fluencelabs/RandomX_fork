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

#pragma once

#include <cstdint>
#include <iostream>
#include <type_traits>
#include "blake2/endian.h"

namespace randomx {

	class Instruction;

	typedef void(Instruction::*InstructionFormatter)(std::ostream&) const;

	enum class InstructionType : uint16_t {
		IADD_RS = 0,
		IADD_M = 1,
		ISUB_R = 2,
		ISUB_M = 3,
		IMUL_R = 4,
		IMUL_M = 5,
		IMULH_R = 6,
		IMULH_M = 7,
		ISMULH_R = 8,
		ISMULH_M = 9,
		IMUL_RCP = 10,
		INEG_R = 11,
		IXOR_R = 12,
		IXOR_M = 13,
		IROR_R = 14,
		IROL_R = 15,
		ISWAP_R = 16,
		FSWAP_R = 17,
		FADD_R = 18,
		FADD_M = 19,
		FSUB_R = 20,
		FSUB_M = 21,
		FSCAL_R = 22,
		FMUL_R = 23,
		FDIV_M = 24,
		FSQRT_R = 25,
		CBRANCH = 26,
		CFROUND = 27,
		ISTORE = 28,
		NOP = 29,
	};

	inline std::ostream& operator<<(std::ostream& os, InstructionType type) {
    switch (type) {
        case InstructionType::IADD_RS:   return os << "IADD_RS";
        case InstructionType::IADD_M:    return os << "IADD_M";
        case InstructionType::ISUB_R:    return os << "ISUB_R";
        case InstructionType::ISUB_M:    return os << "ISUB_M";
        case InstructionType::IMUL_R:    return os << "IMUL_R";
        case InstructionType::IMUL_M:    return os << "IMUL_M";
        case InstructionType::IMULH_R:   return os << "IMULH_R";
        case InstructionType::IMULH_M:   return os << "IMULH_M";
        case InstructionType::ISMULH_R:  return os << "ISMULH_R";
        case InstructionType::ISMULH_M:  return os << "ISMULH_M";
        case InstructionType::IMUL_RCP:  return os << "IMUL_RCP";
        case InstructionType::INEG_R:    return os << "INEG_R";
        case InstructionType::IXOR_R:    return os << "IXOR_R";
        case InstructionType::IXOR_M:    return os << "IXOR_M";
        case InstructionType::IROR_R:    return os << "IROR_R";
        case InstructionType::IROL_R:    return os << "IROL_R";
        case InstructionType::ISWAP_R:   return os << "ISWAP_R";
        case InstructionType::FSWAP_R:   return os << "FSWAP_R";
        case InstructionType::FADD_R:    return os << "FADD_R";
        case InstructionType::FADD_M:    return os << "FADD_M";
        case InstructionType::FSUB_R:    return os << "FSUB_R";
        case InstructionType::FSUB_M:    return os << "FSUB_M";
        case InstructionType::FSCAL_R:   return os << "FSCAL_R";
        case InstructionType::FMUL_R:    return os << "FMUL_R";
        case InstructionType::FDIV_M:    return os << "FDIV_M";
        case InstructionType::FSQRT_R:   return os << "FSQRT_R";
        case InstructionType::CBRANCH:   return os << "CBRANCH";
        case InstructionType::CFROUND:   return os << "CFROUND";
        case InstructionType::ISTORE:    return os << "ISTORE";
        case InstructionType::NOP:       return os << "NOP";
    }
    return os << "Unknown InstructionType(" << static_cast<uint16_t>(type) << ")";
}

	class Instruction {
	public:
		uint32_t getImm32() const {
			return load32(&imm32);
		}
		void setImm32(uint32_t val) {
			return store32(&imm32, val);
		}
		const char* getName() const {
			return names[opcode];
		}
		friend std::ostream& operator<<(std::ostream& os, const Instruction& i) {
			i.print(os);
			return os;
		}
		int getModMem() const {
			return mod % 4; //bits 0-1
		}
		int getModShift() const {
			return (mod >> 2) % 4; //bits 2-3
		}
		int getModCond() const {
			return mod >> 4; //bits 4-7
		}
		void setMod(uint8_t val) {
			mod = val;
		}

		uint8_t opcode;
		uint8_t dst;
		uint8_t src;
		uint8_t mod;
		uint32_t imm32;
	private:
		void print(std::ostream&) const;
		static const char* names[256];
		static InstructionFormatter engine[256];
		void genAddressReg(std::ostream& os, int) const;
		void genAddressImm(std::ostream& os) const;
		void genAddressRegDst(std::ostream&, int) const;
		void h_IADD_RS(std::ostream&) const;
		void h_IADD_M(std::ostream&) const;
		void h_ISUB_R(std::ostream&) const;
		void h_ISUB_M(std::ostream&) const;
		void h_IMUL_R(std::ostream&) const;
		void h_IMUL_M(std::ostream&) const;
		void h_IMULH_R(std::ostream&) const;
		void h_IMULH_M(std::ostream&) const;
		void h_ISMULH_R(std::ostream&) const;
		void h_ISMULH_M(std::ostream&) const;
		void h_IMUL_RCP(std::ostream&) const;
		void h_INEG_R(std::ostream&) const;
		void h_IXOR_R(std::ostream&) const;
		void h_IXOR_M(std::ostream&) const;
		void h_IROR_R(std::ostream&) const;
		void h_IROL_R(std::ostream&) const;
		void h_ISWAP_R(std::ostream&) const;
		void h_FSWAP_R(std::ostream&) const;
		void h_FADD_R(std::ostream&) const;
		void h_FADD_M(std::ostream&) const;
		void h_FSUB_R(std::ostream&) const;
		void h_FSUB_M(std::ostream&) const;
		void h_FSCAL_R(std::ostream&) const;
		void h_FMUL_R(std::ostream&) const;
		void h_FDIV_M(std::ostream&) const;
		void h_FSQRT_R(std::ostream&) const;
		void h_CBRANCH(std::ostream&) const;
		void h_CFROUND(std::ostream&) const;
		void h_ISTORE(std::ostream&) const;
		void h_NOP(std::ostream&) const;
	};

	static_assert(sizeof(Instruction) == 8, "Invalid size of struct randomx::Instruction");
	static_assert(std::is_standard_layout<Instruction>(), "randomx::Instruction must be a standard-layout struct");
}