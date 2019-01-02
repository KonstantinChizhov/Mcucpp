
#include <gtest/gtest.h>
#include <data_buffer.h>

using namespace Mcucpp;


TEST(DataBuffer, InsertFront)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	EXPECT_TRUE(buffer.BufferList() != 0);
	EXPECT_EQ(10, buffer.BufferList()->Size());
}

TEST(DataBuffer, InsertBack)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertBack(10);
	ASSERT_TRUE(res);
	EXPECT_TRUE(buffer.BufferList() != 0);
	EXPECT_EQ(10, buffer.BufferList()->Size());
}


TEST(DataBuffer, Insert2)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	res = buffer.InsertBack(10);
	ASSERT_TRUE(res);
	EXPECT_EQ(20, buffer.BufferList()->Size());
	EXPECT_EQ(20, buffer.Size());
}

TEST(DataBuffer, InsertFront2Times)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	bool res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	res = buffer.InsertFront(10);
	ASSERT_TRUE(res);
	EXPECT_EQ(20, buffer.Size());
	EXPECT_EQ(2, buffer.Parts());
}

TEST(DataBuffer, ReadWrite)
{
	DataBuffer buffer;
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


TEST(DataBuffer, AttachFront)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachFront(DataChunk::GetNew(10));
	buffer.AttachFront(DataChunk::GetNew(10));
	EXPECT_EQ(20, buffer.Size());
}

TEST(DataBuffer, AttachBack)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachBack(DataChunk::GetNew(10));
	buffer.AttachBack(DataChunk::GetNew(10));
	EXPECT_EQ(20, buffer.Size());
}

TEST(DataBuffer, DetachFront)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	EXPECT_TRUE(buffer.InsertBack(10));
	EXPECT_TRUE(buffer.InsertFront(10));
	EXPECT_EQ(20, buffer.Size());
	EXPECT_EQ(2, buffer.Parts());
	DataChunk * buf = buffer.DetachFront();
	EXPECT_TRUE(buf != NULL);
	EXPECT_EQ(1, buffer.Parts());
	DataChunk * buf2 = buffer.DetachFront();
	EXPECT_TRUE(buf2 != NULL);
	EXPECT_EQ(0, buffer.Parts());
}

TEST(DataBuffer, SeekAndTell)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	buffer.AttachFront(DataChunk::GetNew(10));
	buffer.AttachFront(DataChunk::GetNew(10));
	buffer.AttachFront(DataChunk::GetNew(10));
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


TEST(DataBuffer, AttachAndInsterFront)
{
	DataBuffer buffer;
	EXPECT_TRUE(buffer.BufferList() == 0);
	
	uint8_t data[100];
	DataChunk chunk(data, 0, sizeof(data));
	buffer.AttachFront(&chunk);
	EXPECT_EQ(0, buffer.Size());
	EXPECT_EQ(1, buffer.Parts());
	
	EXPECT_EQ(buffer.BufferList(), &chunk);
	buffer.InsertBack(10);
	EXPECT_EQ(10, buffer.Size());
	EXPECT_EQ(1, buffer.Parts());
	
	buffer.InsertBack(80);
	EXPECT_EQ(90, buffer.Size());
	EXPECT_EQ(1, buffer.Parts());
	
	buffer.InsertBack(10);
	EXPECT_EQ(100, buffer.Size());
	EXPECT_EQ(1, buffer.Parts());
}