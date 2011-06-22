#pragma once

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
