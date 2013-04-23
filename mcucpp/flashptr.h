#pragma once

#if defined(__ICCAVR__)

#define FLASH_PTR(PTR_TYPE) PTR_TYPE const __flash*

#define FLASH_STORAGE __flash

template<class PtrT>
inline  PtrT __flash* MakeFlashPtr( PtrT __flash* ptr){return ptr;}

#elif defined(__AVR__)

#include <avr/pgmspace.h>

template<class T, class PtrType = const T*>
class ProgmemPtr
{
	typedef ProgmemPtr Self;
public:
	ProgmemPtr(const T *address=0)
		:_address(address)
	{
	}

	inline Self& operator ++()
	{
		_address++;
		return *this;
	}

	inline Self operator ++(int)
	{
		Self tmp = *this;
		_address++;
		return tmp;
	}

	inline Self& operator --()
	{
		_address--;
		return *this;
	}

	inline Self operator --(int)
	{
		Self tmp = *this;
		_address--;
		return tmp;
	}

	inline Self& operator +=(size_t value)
	{
		_address += value;
		return *this;
	}

	inline Self& operator -=(size_t value)
	{
		_address -= value;
		return *this;
	}

	inline Self operator +(size_t value)const
	{
		return Self(_address + value);
	}

	inline Self operator -(size_t value)const
	{
		return Self(_address - value);
	}

	inline int operator +(Self &other)const
	{
		return (_address + other._address);
	}

	inline int operator -(Self &other)const
	{
		return (_address - other._address);
	}

	inline bool operator !=(const Self &other) const
	{
		return _address != other._address;
	}

	inline bool operator ==(const Self &other) const
	{
		return _address == other._address;
	}

	inline operator bool() const
	{
		return _address != 0;
	}

	inline T operator [](int index)
	{
		T result;
		uint8_t *ptr = static_cast<uint8_t*>(&result);

		for(unsigned i = 0; i<sizeof(T); ++i)
			ptr[i] = pgm_read_byte((const uint8_t* const)(_address + index) + i);
		return result;
	}

	inline const T operator *()const
	{
		union
		{
			T value;
			uint8_t bytes[sizeof(T)];
		}bytesToValue;

		for(unsigned i = 0; i<sizeof(T); ++i)
			bytesToValue.bytes[i] =
					pgm_read_byte((const uint8_t* const)(_address) + i);
		return bytesToValue.value;
	}

protected:
	PtrType _address;
};

#define FLASH_STORAGE PROGMEM
#define FLASH_PTR(PTR_TYPE) ProgmemPtr<PTR_TYPE>

template<class PtrT>
inline ProgmemPtr<PtrT> MakeFlashPtr(const PtrT *ptr){return ProgmemPtr<PtrT>(ptr);}

#else

template<class PtrT>
inline const PtrT * MakeFlashPtr(const PtrT *ptr){return ptr;}
#define FLASH_STORAGE
#define FLASH_PTR(PTR_TYPE) const PTR_TYPE*

#endif
