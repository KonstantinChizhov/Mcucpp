//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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
		STRUCT_TYPE* operator->(){return ((STRUCT_TYPE *)(STRUCT_PTR));}\
	}

#define IO_BITFIELD_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE, BITFIELD_OFFSET, BITFIELD_LENGTH) \
	struct CLASS_NAME\
	{\
		typedef DATA_TYPE DataT;\
		static const DataT Mask = ((DataT(1u) << BITFIELD_LENGTH) - 1);\
		static DataT Get(){return (REG_NAME >> BITFIELD_OFFSET) & Mask;}\
		static void Set(DataT value){REG_NAME = (REG_NAME & ~(Mask << BITFIELD_OFFSET)) | ((value & Mask) << BITFIELD_OFFSET);}\
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
	
}
