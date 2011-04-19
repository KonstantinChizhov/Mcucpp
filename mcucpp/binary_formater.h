#pragma once

template<class DATA_SOURCE>
class BinaryFormater :public DATA_SOURCE
{
public:
	using DATA_SOURCE::Write;
	using DATA_SOURCE::Read;

	static void Write(uint32_t value)
	{
		Int32 i;
		i.Dword = value;
		DATA_SOURCE::Putch(i.Bytes[0]);
		DATA_SOURCE::Putch(i.Bytes[1]);
		DATA_SOURCE::Putch(i.Bytes[2]);
		DATA_SOURCE::Putch(i.Bytes[3]);
	}

	static void Write(uint16_t value)
	{
		DATA_SOURCE::Write(value & 0xff);
		DATA_SOURCE::Write((value >>= 8) & 0xff);
	}

	template<class T>
	static void Write(const T &value)
	{
		Write(&value, sizeof(T));
	}

	static void Write(const void *data, size_t size)
	{
		for(size_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Write(((const uint8_t*)data)[i]);
		}
	}

	static void Puts(const char *str)
	{
		while(*str)
		{
			DATA_SOURCE::Write(*str);
			str++;
		}
	}

	static void Read(void *data, size_t size)
	{
		for(size_t i=0; i<size; ++i)
		{
			((uint8_t*)data)[i] = DATA_SOURCE::Read();
		}
	}

	template<class T>
	static void Read(T &value)
	{
		Read(&value, sizeof(T));
	}

	static uint32_t ReadU32()
	{
		Int32 i;
		i.Bytes[0] = DATA_SOURCE::Read();
		i.Bytes[1] = DATA_SOURCE::Read();
		i.Bytes[2] = DATA_SOURCE::Read();
		i.Bytes[3] = DATA_SOURCE::Read();
		return i.Dword;
	}

	static uint16_t ReadU16()
	{
		Int16 i;
		i.Bytes[0] = DATA_SOURCE::Read();
		i.Bytes[1] = DATA_SOURCE::Read();
		return i.Word;
	}
};
