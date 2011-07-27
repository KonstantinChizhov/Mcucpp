#pragma once

#if defined(__ICCAVR__) 

#define FLASH_PTR(PTR) (PTR)
#define FLASH __flash

template<class PtrT>
inline __flash PtrT * FLASH_PTR(__flash PtrT *ptr){return ptr;}

#elif defined(__AVR__)

#include <avr/pgmspace.h>

template<class T, class PtrType = T*>
class ProgmemPtr
{
	typedef ProgmemPtr Self;
public:
	ProgmemPtr(T *address=0)
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

	inline Self& operator +=(int value)
	{
		_address += value;
		return *this;
	}

	inline Self& operator -=(int value)
	{
		_address -= value;
		return *this;
	}

	inline Self operator +(int value)const
	{
		return Self(_address + value);
	}

	inline Self operator -(int value)const
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
		return _address == 0;
	}

	inline const T operator *()const
	{
		union
		{
			T value;
			uint8_t bytes[sizeof(T)];
		}bytesToValue;
		for(unsigned i = 0; i<sizeof(T); ++i)
			bytesToValue.bytes[i] = pgm_read_byte((uint8_t* const)(_address) + i);
		return bytesToValue.value;
	}

private:
	PtrType _address;
};

#define FLASH PROGMEM

template<class PtrT>
inline ProgmemPtr<PtrT> FLASH_PTR(PtrT *ptr){return ProgmemPtr<PtrT>(ptr);}

#else

inline const PtrT * FLASH_PTR(const PtrT *ptr){return ptr;}
#define FLASH const

#endif
