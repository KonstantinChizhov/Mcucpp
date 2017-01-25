
#include <gtest/gtest.h>
#include <net/net_buffer.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;

TEST(NetBuffer, InsertFront)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	EXPECT_TRUE(buffer.BufferList() != 0);
	EXPECT_EQ(10, buffer.BufferList()->Size());
}

TEST(NetBuffer, InsertBack)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertBack(10);
	ASSERT_TRUE(res);
	EXPECT_TRUE(buffer.BufferList() != 0);
	EXPECT_EQ(10, buffer.BufferList()->Size());
}


TEST(NetBuffer, Insert2)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	res = buffer.InsertBack(10);
	ASSERT_TRUE(res);
	EXPECT_EQ(20, buffer.BufferList()->Size());
	EXPECT_EQ(20, buffer.Size());
}

TEST(NetBuffer, InsertFront2Times)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	EXPECT_EQ(20, buffer.Size());
	EXPECT_EQ(2, buffer.Parts());
}

TEST(NetBuffer, ReadWrite)
{
	NetBuffer buffer;
	ASSERT_TRUE(buffer.InsertFront(10));
	ASSERT_TRUE(buffer.InsertBack(10));
	EXPECT_EQ(20, buffer.BufferList()->Size());
	buffer.Seek(0);
	
	for(int i=0; i < 20; i++)
		buffer.Write((uint8_t)i);
	
	EXPECT_EQ(0, buffer.Read());
	EXPECT_EQ(0, buffer.Read());
	
	buffer.Seek(0);
	
	for(int i=0; i < 20; i++)
		EXPECT_EQ(i, buffer.Read());
	
	buffer.Seek(0);
	buffer.WriteU32Be(0x11223344);
	buffer.WriteU32Be(0xaabbccdd);
	
	buffer.Seek(0);
	EXPECT_EQ(0x11223344, buffer.ReadU32Be());
	EXPECT_EQ(0xaabbccdd, buffer.ReadU32Be());
}


TEST(NetBuffer, AttachFront)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachFront(DataBuffer::GetNew(10));
	buffer.AttachFront(DataBuffer::GetNew(10));
	EXPECT_EQ(20, buffer.Size());
}

TEST(NetBuffer, AttachBack)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachBack(DataBuffer::GetNew(10));
	buffer.AttachBack(DataBuffer::GetNew(10));
	EXPECT_EQ(20, buffer.Size());
}

TEST(NetBuffer, DetachFront)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	EXPECT_TRUE(buffer.InsertBack(10));
	EXPECT_TRUE(buffer.InsertFront(10));
	EXPECT_EQ(20, buffer.Size());
	EXPECT_EQ(2, buffer.Parts());
	DataBuffer * buf = buffer.DetachFront();
	EXPECT_TRUE(buf != NULL);
	EXPECT_EQ(1, buffer.Parts());
	DataBuffer * buf2 = buffer.DetachFront();
	EXPECT_TRUE(buf2 != NULL);
	EXPECT_EQ(0, buffer.Parts());
}

TEST(NetBuffer, SeekAndTell)
{
	NetBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachFront(DataBuffer::GetNew(10));
	buffer.AttachFront(DataBuffer::GetNew(10));
	buffer.AttachFront(DataBuffer::GetNew(10));
	buffer.Seek(0);
	EXPECT_EQ(0, buffer.Tell());
	
	buffer.Seek(10);
	EXPECT_EQ(10, buffer.Tell());
	
	buffer.Seek(15);
	EXPECT_EQ(15, buffer.Tell());
	
	buffer.Seek(20);
	EXPECT_EQ(20, buffer.Tell());
	
	buffer.Seek(25);
	EXPECT_EQ(25, buffer.Tell());
}