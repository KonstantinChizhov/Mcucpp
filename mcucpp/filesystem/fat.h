#include <static_assert.h>#include <binary_stream.h>
#include <iostream>
#include <iomanip>
namespace Mcucpp{	namespace Fat	{
        static const uint16_t BootSignatureOff = 510;
        static const uint16_t BootSignatureVal = 0xaa55;

        enum FileAttributes
        {
            ATTR_READ_ONLY    = 0x01,
            ATTR_HIDDEN       = 0x02,
            ATTR_SYSTEM       = 0x04,
            ATTR_VOLUME_ID    = 0x08,
            ATTR_DIRECTORY    = 0x10,
            ATTR_ARCHIVE      = 0x20,
            ATTR_LONG_NAME    = 0x0f
        };        enum
        {
            DIR_ENTRY_SIZE    = 0x32,            EMPTY             = 0x00,            DELETED           = 0xe5
        };
        enum FatType
        {
            None = 0,
            Fat12,
            Fat16,
            Fat32
        };

		struct FatInfo
		{
			uint8_t  sectorPerCluster;
			uint8_t  numberofFATs;
			uint16_t bytesPerSector;			uint16_t reservedSectorCount;			uint16_t rootEntCnt;
			uint16_t rootDirSectors;
			uint32_t totalSectors;			uint32_t hiddenSectors;			uint32_t FATsize;
			uint32_t firstSector;
			uint32_t rootCluster;
			uint32_t countofClusters;
			uint32_t firstDataSector;
			uint32_t dataSectors;
			FatType type;
		};

		template<class Dev>
		class FatFs
		{
			static FatInfo _fat;
			static uint32_t ClasterToSector(uint32_t cluster)
			{
				return (((cluster - 2) * _fat.sectorPerCluster) + _fat.firstDataSector);
			}

			static uint32_t FatSector(uint32_t cluster, unsigned fatNumber)
			{
                if(_fat.type == Fat12)
                {

                } else if(_fat.type == Fat16)
                {
                    cluster *= 2;
                }else
                {
                    cluster *= 4;
                }
                return 0;
			}

		public:
		typedef BinaryStream<Dev> Stream;
			static bool Mount();
			static void StartReadingDir();
			static bool NextDirEntry();
		};

		template<class Dev>
		FatInfo FatFs<Dev>::_fat;

		template<class Dev>
		void FatFs<Dev>::StartReadingDir()
		{
            if(_fat.type != Fat32)
            {
                for(int c = 0; c < 1 /*_fat.rootDirSectors*/; c++)
                {
                    Stream::StartRead(_fat.rootCluster + c);
                    for(int i = 0 ; i < 512 / DIR_ENTRY_SIZE; i++)
                    {
                        char name[12];
                        Stream::Read(name, 11);
                        name[11] = 0;
                        uint8_t attr = Stream::Read();
                        Stream::Ignore(16);
                        uint32_t size = Stream::ReadU32Le();
                        std::cout << std::left << std::setw(16) << name << "\t" << std::hex << (unsigned)attr << "\t" << std::dec << size << std::endl;
                    }
                    Stream::EndRead();
                }
            }
		}

		template<class Dev>		bool FatFs<Dev>::Mount()
		{
			if(!Dev::Activate())
				return false;

			Stream::StartRead(0);
			uint8_t c = Stream::Read();
			if(c != 0xE9 && c != 0xEB) // MBR?
			{
				Stream::ReadTo(446 + 8);
				_fat.firstSector = Stream::ReadU32Le();
				Stream::ReadTo(510);
				uint16_t sig = Stream::ReadU16Le();
				Stream::EndRead();
				if(sig != BootSignatureVal)
				{
					return false;
				}
				Stream::StartRead(_fat.firstSector);
				uint8_t c = Stream::Read();
				if(c != 0xE9 && c != 0xEB)
					return false;
			}
			Stream::ReadTo(11);
			_fat.bytesPerSector = Stream::ReadU16Le();			_fat.sectorPerCluster = Stream::Read();			_fat.reservedSectorCount = Stream::ReadU16Le();			_fat.numberofFATs = Stream::Read();			_fat.rootEntCnt = Stream::ReadU16Le();			_fat.totalSectors = Stream::ReadU16Le();
			/*uint8_t  mediaType = */Stream::Read();			_fat.FATsize = Stream::ReadU16Le();			/*uint16_t sectorsPerTrack = */Stream::ReadU16Le();			/*uint16_t numberofHeads = */Stream::ReadU16Le();			_fat.hiddenSectors = Stream::ReadU32Le();			uint32_t totalSectors_F32 = Stream::ReadU32Le();

			if(_fat.totalSectors == 0)
				_fat.totalSectors = totalSectors_F32;

			if(_fat.FATsize == 0)
			{
				_fat.FATsize = Stream::ReadU32Le();
				/*uint16_t extFlags = */Stream::ReadU16Le();				/*uint16_t FSversion = */Stream::ReadU16Le();				_fat.rootCluster = Stream::ReadU32Le();
			}			Stream::ReadTo(510);
			uint16_t sig = Stream::ReadU16Le();
			Stream::EndRead();
			if(sig != 0xaa55)
			{
				return false;
			}

            _fat.rootDirSectors = (_fat.rootEntCnt * 32 + _fat.bytesPerSector - 1) / _fat.bytesPerSector;

			_fat.firstDataSector = _fat.rootDirSectors + _fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs;

			uint32_t dataSec = _fat.totalSectors - (_fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs + _fat.rootDirSectors);
			_fat.countofClusters = dataSec / _fat.sectorPerCluster;
            if(_fat.rootCluster == 0)
                _fat.rootCluster = (_fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs);

            if(_fat.countofClusters < 4085)
            {
                _fat.type = Fat12;
            } else if(_fat.countofClusters < 65525)
            {
                _fat.type = Fat16;
            } else
            {
                _fat.type = Fat32;
            }
#define Prn(x) std::cout << std::left << std::setw(24) << #x " = " << (uint32_t)x << "\n"

Prn(_fat.bytesPerSector);
Prn(_fat.sectorPerCluster);
Prn(_fat.reservedSectorCount);
Prn(_fat.numberofFATs);
Prn(_fat.rootEntCnt);
Prn(_fat.totalSectors);
Prn(_fat.FATsize);
Prn(_fat.hiddenSectors);
Prn(_fat.rootCluster);
Prn(_fat.countofClusters);
Prn(_fat.firstDataSector);
Prn(_fat.rootDirSectors);
			return true;
		}	}}
