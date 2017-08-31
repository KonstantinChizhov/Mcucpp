
#include <gtest/gtest.h>
#include <crc.h>

TEST(Crc, crc16)
{
	typedef Mcucpp::Crc16 myCrc;
	const char * testMesaage = myCrc::CheckMessage();
	uint16_t crc = Mcucpp::ComputeCrc<myCrc>((uint8_t*)testMesaage, strlen(testMesaage));
	const uint16_t checkCrc = myCrc::Check;
	EXPECT_EQ(checkCrc, crc);
}

TEST(Crc, Crc16Citt)
{
	typedef Mcucpp::Crc16Citt myCrc;
	const char * testMesaage = myCrc::CheckMessage();
	uint16_t crc = Mcucpp::ComputeCrc<myCrc>((uint8_t*)testMesaage, strlen(testMesaage));
	const uint16_t checkCrc = myCrc::Check;
	EXPECT_EQ(checkCrc, crc);
}

TEST(Crc, crc32)
{
	typedef Mcucpp::Crc32 myCrc;
	const char * testMesaage = myCrc::CheckMessage();
	uint32_t crc = Mcucpp::ComputeCrc<myCrc>((uint8_t*)testMesaage, strlen(testMesaage));
	const uint32_t checkCrc = myCrc::Check;
	EXPECT_EQ(checkCrc, crc);
}

TEST(Crc, XModem)
{
	typedef Mcucpp::XModemCrc myCrc;
	const char * testMesaage = myCrc::CheckMessage();
	uint16_t crc = Mcucpp::ComputeCrc<myCrc>((uint8_t*)testMesaage, strlen(testMesaage));
	const uint16_t checkCrc = myCrc::Check;
	EXPECT_EQ(checkCrc, crc);
}

TEST(Crc, Dallas)
{
	typedef Mcucpp::DallasCrc myCrc;
	const char * testMesaage = myCrc::CheckMessage();
	uint8_t crc = Mcucpp::ComputeCrc<myCrc>((uint8_t*)testMesaage, strlen(testMesaage));
	const uint8_t checkCrc = myCrc::Check;
	EXPECT_EQ(checkCrc, crc);
}
