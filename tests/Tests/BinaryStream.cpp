
#include <gtest.h>
#include <binary_stream.h>

uint8_t buffer[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
size_t pos = 0;

class BufferReader
{
public:
	static uint8_t Read()
	{
		if(pos < sizeof(buffer))
			return buffer[pos++];
		return 0;
	}

	static void Write(uint8_t value)
	{
		if(pos < sizeof(buffer))
			buffer[pos++] = value;
	}

	static void Reset()
	{
		pos = 0;
	}
};
using namespace Mcucpp;
typedef BinaryStream<BufferReader> stream;

TEST(Binary, Read)
{
	stream::Reset();
	uint32_t val32 = stream::ReadU32Be();
	EXPECT_EQ(0x01020304u, val32);
	stream::Reset();
	val32 = stream::ReadU32Le();
	EXPECT_EQ(0x04030201u, val32);
	stream::Reset();
	uint16_t val16 = stream::ReadU16Be();
	EXPECT_EQ(0x0102u, val16);
	stream::Reset();
	val16 = stream::ReadU16Le();
	EXPECT_EQ(0x0201u, val16);
	stream::Reset();
	char buf[10] = {0};
	stream::Read(buf, 8);
	EXPECT_STREQ("\x1\x2\x3\x4\x5\x6\x7\x8", buf);
}

TEST(Binary, Write)
{
	uint32_t val32;
	uint16_t val16;
	stream::Reset();
	stream::WriteU32Be(0x01020304);
	stream::Reset();
	val32 = stream::ReadU32Be();
	EXPECT_EQ(0x01020304u, val32);

	stream::Reset();
	stream::WriteU32Le(0x01020304);
	stream::Reset();
	val32 = stream::ReadU32Le();
	EXPECT_EQ(0x01020304u, val32);

	stream::Reset();
	stream::WriteU16Be(0x0102);
	stream::Reset();
	val16 = stream::ReadU16Be();
	EXPECT_EQ(0x0102u, val16);

	stream::Reset();
	stream::WriteU16Le(0x0102);
	stream::Reset();
	val16 = stream::ReadU16Le();
	EXPECT_EQ(0x0102u, val16);

	stream::Reset();
	stream::Write("Hello world", sizeof("Hello world"));
	EXPECT_STREQ("Hello world", (char*)buffer);
}

TEST(Binary, Ignore)
{
	uint8_t byte;
	stream::Reset();
	stream::Write("\x1\x2\x3\x4\x5\x6\x7\x8", 8);
	stream::Reset();
	byte = stream::Ignore(5);
	EXPECT_EQ(0x05u, byte);
	stream::Reset();
	byte = stream::Ignore(8, 6);
	EXPECT_EQ(0x06u, byte);
	stream::Reset();
	stream::Write("\xff\xff\xff\xff\xff\xff\x7\x8", 8);
	stream::Reset();
	byte = stream::IgnoreWhile(8, '\xff');
	EXPECT_EQ(0x07u, byte);
}

