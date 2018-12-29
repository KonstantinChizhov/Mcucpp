//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#pragma once

#include <stddef.h>
#include <compiler.h>

namespace Mcucpp
{
#define IO_REG_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE) \
	struct CLASS_NAME\
	{\
		typedef DATA_TYPE DataT;\
		static DataT Get(){return REG_NAME;}\
		static void Set(DataT value){REG_NAME = value;}\
		static void Or(DataT value){REG_NAME |= value;}\
		static void And(DataT value){REG_NAME &= value;}\
		static void Xor(DataT value){REG_NAME ^= value;}\
		static void AndOr(DataT andMask, DataT orMask){REG_NAME = (REG_NAME & andMask) | orMask;}\
		template<int Bit>\
		static bool BitIsSet(){return REG_NAME & (1 << Bit);}\
		template<int Bit>\
		static bool BitIsClear(){return !(REG_NAME & (1 << Bit));}\
	}

#define I_REG_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE) \
	struct CLASS_NAME\
	{\
		typedef DATA_TYPE DataT;\
		static DataT Get(){return REG_NAME;}\
	}

#define IO_STRUCT_WRAPPER(STRUCT_PTR, CLASS_NAME, STRUCT_TYPE) \
	struct CLASS_NAME\
	{\
		typedef STRUCT_TYPE DataT;\
		static STRUCT_TYPE* Get(){return STRUCT_PTR;}\
		STRUCT_TYPE* operator->(){return ((STRUCT_TYPE *)(STRUCT_PTR));}\
	}

#define IO_BITFIELD_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE, BITFIELD_OFFSET, BITFIELD_LENGTH) \
	struct CLASS_NAME\
	{\
		typedef DATA_TYPE DataT;\
		typedef typename std::remove_cv<decltype(REG_NAME)>::type RegT;\
		static constexpr RegT Mask = (((RegT)(1u) << BITFIELD_LENGTH) - 1);\
		static DataT Get(){return static_cast<DataT>((REG_NAME >> BITFIELD_OFFSET) & Mask);}\
		static void Set(DataT value){REG_NAME = (REG_NAME & ~(Mask << BITFIELD_OFFSET)) | (((RegT)value & Mask) << BITFIELD_OFFSET);}\
	}


// A stub for IO register
// Any data written is ignored
// Always reads as zero
template<class DATA_TYPE = unsigned char>
	struct NullReg
	{
		typedef DATA_TYPE DataT;
		static DataT Get(){return DataT(0);}
		static void Set(DataT){}
		static void Or(DataT){}
		static void And(DataT){}
		static void Xor(DataT){}
		static void AndOr(DataT, DataT){}
		template<int Bit>
		static bool BitIsSet(){return false;}
		template<int Bit>
		static bool BitIsClear(){return true;}
	};

	template<size_t RegAddr, class DATA_TYPE = unsigned char>
	struct IoReg
	{
		typedef DATA_TYPE DataT;

		static volatile DataT& Value(){ return *static_cast<DataT*>(RegAddr);}
		static DataT Get(){return Value();}
		static void Set(DataT value){Value() = value;}
		static void Or(DataT value){Value() |= value;}
		static void And(DataT value){Value() &= value;}
		static void Xor(DataT value){Value() ^= value;}
		static void AndOr(DataT andMask, DataT orMask){Value() = (Value() & andMask) | orMask;}
		template<int Bit>
		static bool BitIsSet(){return Value() & (1 << Bit);}
		template<int Bit>
		static bool BitIsClear(){return !(Value() & (1 << Bit));}
	};

	template<size_t RegAddr, class STRUCT_TYPE>
	struct IoStruct
	{
		typedef STRUCT_TYPE DataT;
		static DataT* Get(){return static_cast<DataT*>(RegAddr);}
		DataT* operator->(){return Get();}
	};

	template<size_t RegAddr, class DATA_TYPE, int BITFIELD_OFFSET, int BITFIELD_LENGTH>
	struct IoBitfield
	{
		typedef DATA_TYPE DataT;
		static volatile DATA_TYPE& Value(){ return *static_cast<DATA_TYPE*>(RegAddr);}
		static constexpr DataT Mask = ((DataT(1) << BITFIELD_LENGTH) - 1);
		static DataT Get(){return (Value() >> BITFIELD_OFFSET) & Mask;}
		static void Set(DataT value){Value() = (Value() & ~(Mask << BITFIELD_OFFSET)) | ((value & Mask) << BITFIELD_OFFSET);}
	};
}
