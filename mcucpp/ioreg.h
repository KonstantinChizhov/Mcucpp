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
	}

