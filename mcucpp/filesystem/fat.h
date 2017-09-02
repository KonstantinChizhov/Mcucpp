#include <static_assert.h>
#include <filesystem/filesystem.h>
#include <filesystem/fscommon.h>
#include <block_device.h>
#include <memory_stream.h>

namespace Mcucpp
{
	namespace Fat
	{
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
		};

		enum
		{
			DIR_ENTRY_SIZE    = 0x20,
			EMPTY             = 0x00,
			DELETED           = 0xe5
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
			FatInfo()
				:sectorPerCluster(0),
				numberofFATs(0),
				bytesPerSector(0),
				reservedSectorCount(0),
				rootEntCnt(0),
				rootDirSectors(0),
				totalSectors(0),
				hiddenSectors(0),
				FATsize(0),
				firstSector(0),
				rootSector(0),
				countofClusters(0),
				firstDataSector(0),
				dataSectors(0)
			{}

			uint8_t  sectorPerCluster;
			uint8_t  numberofFATs;
			uint16_t bytesPerSector;
			uint16_t reservedSectorCount;
			uint16_t rootEntCnt;
			uint16_t rootDirSectors;
			uint32_t totalSectors;
			uint32_t hiddenSectors;
			uint32_t FATsize;
			uint32_t firstSector;
			uint32_t rootSector;
			uint32_t countofClusters;
			uint32_t firstDataSector;
			uint32_t dataSectors;
			uint32_t eocMarker;
			FatType type;
		};

		class FatFs : public Mcucpp::Fs::IFsDriver
		{
			FatInfo _fat;
			Mcucpp::IBlockDevice &_device;
			Fs::ErrorCode _lastError;
			uint32_t _cachedSector;
			enum {SectorSize = Fs::DefaultBlockSize, LfnNameChunkBytes = 26};
			uint8_t *_sectorBuffer;
			uint8_t *_pathBuffer;
		private:
			uint32_t ClusterToSector(uint32_t cluster);
			uint32_t SectorToClaster(uint32_t sector);
			uint32_t ReadFat(uint32_t cluster, unsigned fatNum);
			bool WriteFat(uint32_t cluster, uint32_t nextCluster);
			MemoryStream ReadSector(uint32_t sector);
			int ReadLfnEntry(MemoryStream &reader, uint8_t *lfnBuffer);
			bool ReadDirEntry(MemoryStream &reader, Fs::FileSystemEntry &entry, int lfnChecksum, uint8_t *lfnBuffer);
		public:
			FatFs(Mcucpp::IBlockDevice &device);

			virtual Fs::ErrorCode GetError();
			virtual Fs::FsNode RootDirectory();
			virtual bool ListDirectory(Fs::FsNode dir, Fs::DirectoryLister &directoryLister);
			virtual uint32_t GetParameter(Fs::FsParams param);
			virtual uint32_t GetBlocksPerNode(Fs::FsNode node);
			virtual Fs::FsNode GetNextChunk(Fs::FsNode node);
			virtual Fs::FsNode AllocNextChunk(Fs::FsNode parentNode);
			virtual Fs::FsNode CreateNode(Fs::FsNode parentDir, Fs::FileAttributes attributes, uint8_t *nodeName);
			virtual bool ReadBlock(Fs::FsNode node, uint8_t *buffer);
			virtual bool WriteBlock(Fs::FsNode node, const uint8_t *buffer);
			virtual bool EndOfFile(Fs::FsNode node);
			virtual bool Mount();
			virtual void Unmount();
		};
	}
}
