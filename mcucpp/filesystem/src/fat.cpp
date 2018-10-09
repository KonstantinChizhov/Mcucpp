#include <filesystem/fat.h>
#include <memory_stream.h>
#include <filesystem/file.h>
#include <template_utils.h>
#include <utf8.h>
#include <data_buffer.h>
#include <new>

namespace Mcucpp
{
namespace Fat
{
	using namespace Mcucpp::Fs;

	size_t DecodeShortName(const uint8_t* shortName, uint8_t *name)
	{
		size_t nameLen = 0;
		for(int i = 0; i < 11; i++)
		{
			if(i == 8)
			{
				*name++ = '.';
				nameLen++;
			}
			if(*shortName == 0x20)
			{
				shortName++;
				continue;
			}
			*name++ = *shortName++;
			nameLen++;
		}
		if(name[-1] == '.')
		{
			name--;
			nameLen--;
		}
		*name = 0;
		return nameLen;
	}

	uint8_t ShortNameCheckSum(const uint8_t* dirEntryName)
	{
		uint8_t result = 0;
		for (int i = 0; i < 11; i++)
		{
			result = ((result & 1) ? 0x80 : 0) + (result >> 1) + dirEntryName[i];
		}
		return result;
	}

	uint32_t FatFs::ClusterToSector(uint32_t cluster)
	{
		return (cluster - 2) * _fat.sectorPerCluster + _fat.firstDataSector;
	}

	uint32_t FatFs::SectorToClaster(uint32_t sector)
	{
		return (sector - _fat.firstDataSector) / _fat.sectorPerCluster + 2;
	}

	FatFs::FatFs(IBlockDevice &device)
		:_device(device),
		_lastError(ErrOK),
		_cachedSector(0xffffffff),
		_sectorBuffer(nullptr)
	{

	}

	MemoryStream FatFs::ReadSector(uint32_t sector)
	{
		if(_cachedSector != sector)
		{
			if(!_device.ReadPage(sector, _sectorBuffer, _fat.bytesPerSector, 0))
			{
				_lastError = ErrIoFailed;
			}
			_cachedSector = sector;
		}
		return MemoryStream(_sectorBuffer, _fat.bytesPerSector);
	}

	ErrorCode FatFs::GetError()
	{
		return _lastError;
	}

	bool FatFs::EndOfFile(Fs::FsNode node)
	{
		return node >= _fat.eocMarker;
	}

	FsNode FatFs::RootDirectory()
	{
		return FsNode(_fat.rootSector);
	}

	bool FatFs::ListDirectory(FsNode dir, DirectoryLister &directoryLister)
	{
		BinaryStream<Fs::File> drectory(*this, dir, -1);

		int lfnChecksum = -1;
		DataBuffer lfnBuffer;

		for(TFileSize dirEntryIndex = 0; !drectory.EndOfFile(); dirEntryIndex += DIR_ENTRY_SIZE)
		{
			drectory.Seek(dirEntryIndex);
			uint8_t marker = drectory.Read();
			if(marker == DELETED)
			{
				continue;
			}
			if(marker == EMPTY)
			{
				_lastError = ErrOK;
				return true;
			}

			drectory.Seek(dirEntryIndex + 11);
			uint8_t attr = drectory.Read();
			if(attr == 0xff)
			{
				continue;
			}
			drectory.Seek(dirEntryIndex);
			FileSystemEntry entry;

			if(attr == ATTR_LONG_NAME) // long name entry
			{
				lfnChecksum = ReadLfnEntry(drectory, lfnBuffer);
				continue;
			}
			else
			{
				if(!ReadDirEntry(drectory, entry, lfnChecksum, lfnBuffer))
				{
					return false;
				}
				lfnChecksum = -1;
			}
			lfnBuffer.Clear();
			bool stop = !directoryLister.DirectoryEntry(entry);

			if(stop)
			{
				_lastError = ErrOK;
				return true;
			}
		}
		_lastError = ErrOK;
		return true;
	}

	int FatFs::ReadLfnEntry(BinaryStream<Fs::File> &reader, DataBuffer &lfnBuffer)
	{
		uint8_t lfnSequence = reader.Read();
		if ((lfnSequence & 0x1F) == 0)
		{
			return -1;
		}
		if((lfnSequence & 0x40) != 0) // last LFN entry for item
		{
			lfnBuffer.Clear();
			lfnSequence &= 0x1F;
			lfnBuffer.InsertFront(lfnSequence * LfnNameChunkBytes);
		}

		lfnBuffer.Seek((lfnSequence - 1) * LfnNameChunkBytes);

		for(int i = 0; i < 5; i++)
		{
			lfnBuffer.WriteU16Le( reader.ReadU16Le());
		}

		reader.Read(); // attributes
		uint8_t type = reader.Read();
		if(type != 0) // not type LFN entry
		{
			return -1;
		}
		int lfnChecksum = reader.Read();

		for(int i = 0; i < 6; i++)
		{
			lfnBuffer.WriteU16Le(reader.ReadU16Le());
		}

		/*uint16_t fstClusLO = */reader.ReadU16Le(); // always 0
		for(int i = 0; i < 2; i++)
		{
			lfnBuffer.WriteU16Le(reader.ReadU16Le());
		}
		return lfnChecksum;
	}

	bool FatFs::ReadDirEntry(BinaryStream<Fs::File> &reader, FileSystemEntry &entry, int lfnChecksum, DataBuffer &lfnBuffer)
	{
		uint8_t shortName[12];
		reader.Read(shortName, 11);
		shortName[11] = 0;
		if(shortName[0] == 0x05)
		{
			shortName[0] = 0xE5;
		}
		uint8_t entryNameCecksum = ShortNameCheckSum(shortName);
		if(lfnChecksum >= 0 && entryNameCecksum == lfnChecksum)
		{
			size_t lfnSize = lfnBuffer.Size() / 2;
			lfnBuffer.Seek(0);
			size_t utf8Size = 0;
			for (size_t i = 0; i < lfnSize; i++)
			{
				uint16_t c = lfnBuffer.ReadU16Le();
				if (!c)
				{
					break;
				}
				utf8Size += Utf8Encoding<uint16_t>::EncodedLen(c);
			}

			lfnBuffer.Seek(0);
			uint8_t *utf8Buffer = new (std::nothrow) uint8_t[utf8Size + 1];
			uint8_t *utf8ptr = utf8Buffer;
			for (size_t i = 0; i < lfnSize; i++)
			{
				uint16_t c = lfnBuffer.ReadU16Le();
				if (!c)
				{
					break;
				}
				Utf8Encoding<uint16_t>::Encode(utf8ptr,c);
			}
			utf8Buffer[utf8Size] = 0;
			entry.SetName(utf8Buffer, utf8Size, false);
		}else
		{
			uint8_t decodedName[12];
			size_t nameLen = DecodeShortName(shortName, decodedName);
			entry.SetName(decodedName, nameLen);
		}

		uint8_t attr = reader.Read();
		uint8_t NTRes = reader.Read();
		uint8_t crtTimeTenth = reader.Read();
		uint16_t crtTime = reader.ReadU16Le();
		uint16_t crtDate = reader.ReadU16Le();
		uint16_t lstAccDate = reader.ReadU16Le();
		uint16_t fstClusHI = reader.ReadU16Le();
		uint16_t wrtTime = reader.ReadU16Le();
		uint16_t wrtDate = reader.ReadU16Le();
		uint16_t fstClusLO  = reader.ReadU16Le();
		uint32_t size = reader.ReadU32Le();

		entry.SetSize(size);
		entry.SetAttributes((Fs::FileAttributes)attr);
		entry.SetNode(ClusterToSector(fstClusLO | (((uint32_t)fstClusHI) << 16)));
		return true;
	}

	uint32_t FatFs::ReadFat(uint32_t sector, unsigned fatNum)
	{
		if(sector == _fat.eocMarker)
			return _fat.eocMarker;

		uint32_t cluster = SectorToClaster(sector);
		uint32_t fatEntryOffset;
		if(_fat.type == Fat12)
		{
			fatEntryOffset = cluster + cluster / 2;
		} else if(_fat.type == Fat16)
		{
			fatEntryOffset = cluster * 2;
		}else
		{
			fatEntryOffset = cluster * 4;
		}

		uint32_t fatEntrySector = _fat.reservedSectorCount +
			(fatEntryOffset / _fat.bytesPerSector) +
			fatNum * _fat.FATsize;
		uint32_t fatEntryOffsetInSector = fatEntryOffset % _fat.bytesPerSector;

		MemoryStream reader = ReadSector(fatEntrySector);
		if(GetError() != ErrOK)
		{
			return _fat.eocMarker;
		}
		reader.Seek(fatEntryOffsetInSector);
		uint32_t result = _fat.eocMarker;

		if(_fat.type == Fat12)
		{
			if(fatEntryOffsetInSector == uint32_t(_fat.bytesPerSector - 1))
			{
				result = reader.Read();
				MemoryStream reader = ReadSector(fatEntrySector + 1);
				result |= uint16_t(reader.Read()) << 8;
			}else{
				result = reader.ReadU16Le();
			}

			if((cluster & 1) != 0)
			{
				result >>= 4;
			}
			result &= 0X0fff;
		} else if(_fat.type == Fat16)
		{
			result = reader.ReadU16Le();
		}else
		{
			result = reader.ReadU32Le() & 0x0fffffff;
		}
		result = ClusterToSector(result);
		return result;
	}

	bool WriteFat(uint32_t cluster, uint32_t nextCluster)
	{
		return false;
	}

	uint32_t FatFs::GetParameter(FsParams param)
	{
		switch(param)
		{
			case BlockSize: return _fat.bytesPerSector;
			case ChunkSize: return _fat.bytesPerSector * _fat.sectorPerCluster;
			case BlocksInChunk: return _fat.sectorPerCluster;
			case TotalBlocks:
			case UsedBlocks:
			default: return 0;
		}
	}

	uint32_t FatFs::GetBlocksPerNode(Fs::FsNode node)
	{
		bool fat12_16_rootDir = _fat.rootDirSectors > 0 && node == RootDirectory();
		if(fat12_16_rootDir)
		{
			return  _fat.rootDirSectors;
		}
		return _fat.sectorPerCluster;
	}

	FsNode FatFs::GetNextChunk(FsNode node)
	{
		return ReadFat((uint32_t)node, 0);
	}

	FsNode FatFs::AllocNextChunk(FsNode parentNode)
	{
		return FsNode(0);
	}

	FsNode FatFs::CreateNode(FsNode parentDir, Fs::FileAttributes attributes, uint8_t *nodeName)
	{
		return FsNode(0);
	}

	bool FatFs::ReadBlock(FsNode node, uint8_t *buffer)
	{
		if(!_device.ReadPage((uint32_t)node, buffer, _fat.bytesPerSector, 0))
		{
			_lastError = ErrIoFailed;
			return false;
		}
		_lastError = ErrOK;
		return true;
	}

	bool FatFs::WriteBlock(FsNode node, const uint8_t *buffer)
	{
		if(!_device.WritePage((uint32_t)node, buffer, _fat.bytesPerSector, 0))
		{
			_lastError = ErrIoFailed;
			return false;
		}
		_lastError = ErrOK;
		return true;
	}

	void FatFs::Unmount()
	{
		if(_sectorBuffer)
		{
			delete [] _sectorBuffer;
		}
		_sectorBuffer = nullptr;
		_fat = FatInfo();
	}

	bool FatFs::Mount()
	{
		_fat = FatInfo();
		if(!_device.Enable())
		{
			_lastError = ErrDeviceNotReady;
			return false;
		}

		if(_sectorBuffer)
		{
			delete [] _sectorBuffer;
		}

		_fat.firstSector = 0;
		_fat.bytesPerSector = SectorSize;
		_sectorBuffer = new (std::nothrow) uint8_t[_fat.bytesPerSector];

		// read boot sector
		MemoryStream reader = ReadSector(_fat.firstSector);
		if(GetError() != ErrOK)
		{
			return false;
		}
		reader.Seek(0);
		uint8_t c = reader.Read();
		if(c != 0xE9 && c != 0xEB) // first setor is MBR
		{
			reader.Seek(446 + 8);
			_fat.firstSector = reader.ReadU32Le();
			reader.Seek(510);
			uint16_t sig = reader.ReadU16Le();

			if(sig != BootSignatureVal)
			{
				_lastError = ErrInvalidFs;
				return false;
			}

			reader = ReadSector(_fat.firstSector);
			if(GetError() != ErrOK)
			{
				return false;
			}
			uint8_t c = reader.Read();
			if(c != 0xE9 && c != 0xEB)
			{
				_lastError = ErrInvalidFs;
				return false;
			}
		}
		reader.Seek(11);
		_fat.bytesPerSector = reader.ReadU16Le();
		_fat.sectorPerCluster = reader.Read();
		_fat.reservedSectorCount = reader.ReadU16Le();
		_fat.numberofFATs = reader.Read();
		_fat.rootEntCnt = reader.ReadU16Le();
		_fat.totalSectors = reader.ReadU16Le();
		/*uint8_t  mediaType = */reader.Read();
		_fat.FATsize = reader.ReadU16Le();
		/*uint16_t sectorsPerTrack = */reader.ReadU16Le();
		/*uint16_t numberofHeads = */reader.ReadU16Le();
		_fat.hiddenSectors = reader.ReadU32Le();
		uint32_t totalSectors_F32 = reader.ReadU32Le();

		if(_fat.bytesPerSector > SectorSize)
		{
			delete [] _sectorBuffer;
			_sectorBuffer = new (std::nothrow) uint8_t[_fat.bytesPerSector];
		}

		if(_fat.totalSectors == 0)
			_fat.totalSectors = totalSectors_F32;

		uint32_t fat32RootCluster = 0;
		if(_fat.FATsize == 0)
		{
			_fat.FATsize = reader.ReadU32Le();
			/*uint16_t extFlags = */reader.ReadU16Le();
			/*uint16_t FSversion = */reader.ReadU16Le();
			fat32RootCluster = reader.ReadU32Le();
		}
		reader.Seek(510);
		uint16_t sig = reader.ReadU16Le();
		if(sig != 0xaa55)
		{
			_lastError = ErrInvalidFs;
			return false;
		}

		_fat.rootDirSectors = (_fat.rootEntCnt * 32 + _fat.bytesPerSector - 1) / _fat.bytesPerSector;
		_fat.firstDataSector = _fat.rootDirSectors + _fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs;
		uint32_t dataSec = _fat.totalSectors - (_fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs + _fat.rootDirSectors);
		_fat.countofClusters = dataSec / _fat.sectorPerCluster;
		if(fat32RootCluster == 0)
		{
			_fat.rootSector = (_fat.reservedSectorCount + _fat.FATsize * _fat.numberofFATs);
		}else
		{
			_fat.rootSector = ClusterToSector(fat32RootCluster);
		}

		if(_fat.countofClusters < 4085)
		{
			_fat.type = Fat12;
			_fat.eocMarker = 0x0FF8;
		} else if(_fat.countofClusters < 65525)
		{
			_fat.type = Fat16;
			_fat.eocMarker = 0xFFF8;
		} else
		{
			_fat.type = Fat32;
			_fat.eocMarker = 0x0FFFFFF8;
		}

		return true;
	}

}
}
