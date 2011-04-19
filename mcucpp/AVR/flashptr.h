#pragma once

#if defined(__ICCAVR__) 

#define FLASH_PTR(PTR) (PTR)
#define FLASH __flash

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

	inline Self operator +(int value)
	{
		return Self(_address + value);
	}

	inline Self operator -(int value)
	{
		return Self(_address - value);
	}

	inline bool operator !=(const Self &other) const
	{
		return _address != other._address;
	}

	inline bool operator ==(const Self &other) const
	{
		return _address == other._address;
	}

	inline const T operator *()const
	{
		union 
		{
			T value;
			uint8_t bytes[sizeof(T)];
		};
		for(unsigned i = 0; i<sizeof(T); ++i)
			bytes[i] = pgm_read_byte(_address + i);
		return value;
	}

private:
	PtrType _address;
};

#define FLASH PROGMEM
#define FLASH_PTR(PTR) ProgmemPtr<typeof(*(PTR))>(PTR)


#else

#define FLASH_PTR(PTR) (PTR)
#define FLASH const

#endif
