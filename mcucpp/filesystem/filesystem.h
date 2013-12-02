#pragma once

#include <stdint.h>

namespace Mcucpp
{
namespace Fs
{
	typedef uint32_t FsNode;
	
	const FsNode EndOfFileNode = 0xffffffff;
	const uint8_t PathDelim = '/';
	
	enum FileAttributes
	{
		Normal = 0
	};

	enum DirectoryEntryType
	{
		DirectoryEntry,
		FileEntry
	};

	enum FsParams
	{
		BlockSize,
		TotalBlocks,
		UsedBlocks
	};
	
	class DirectoryLister
	{
	public:
		virtual bool DirectoryEntry(const uint8_t *name, DirectoryEntryType directoryEntryType, FileAttributes attributes, FsNode node)=0;
	};

	
	class IFsDriver
	{
	public:
		virtual FsNode RootDirectory()=0;
		virtual bool ListDirectory(FsNode dir, DirectoryLister &directoryLister)=0;
		virtual uint32_t GetParameter(FsParams param)=0;
		virtual FsNode NextBlock(FsNode node)=0;
		virtual FsNode AllocBlock(FsNode parentNode)=0;
		virtual FsNode CreateNode(FsNode parentDir, DirectoryEntryType type, uint8_t *nodeName)=0;
		virtual bool ReadBlock(FsNode node, uint8_t *buffer)=0;
		virtual bool WriteBlock(FsNode node, const uint8_t *buffer)=0;
	};
	
	
	class FindNodeLister :public DirectoryLister
	{
		const uint8_t *_filePath;
		IFsDriver &_driver;
		bool _found;
		bool _pathMatched;
		FsNode _currentNode;
		
	public:
		FindNodeLister(const uint8_t *filePath, IFsDriver &driver)
			:_filePath(filePath),
			_driver(driver),
			_found(false),
			_pathMatched(false)
		{
			_currentNode = _driver.RootDirectory();
		}
		
		FsNode Find()
		{
			do
			{
				if(!_driver.ListDirectory(_currentNode, *this))
					return EndOfFileNode;
				if(!_pathMatched)
					return EndOfFileNode;
			}while(!_found);
			return _currentNode;
		}
		
		virtual bool DirectoryEntry(const uint8_t *name, DirectoryEntryType directoryEntryType, FileAttributes attributes, FsNode node)
		{
			if(PathElementMatch(name))
			{
				_currentNode = node;
				_pathMatched = true;
				return false;
			}
			else
			{
				_pathMatched = false;
			}
			return true;
		}
		
		bool PathElementMatch(const uint8_t *name)
		{
			const uint8_t *pathPtr = _filePath;
			while(*pathPtr == PathDelim)
			{
				pathPtr++;
			}

			do
			{
				if(*pathPtr == PathDelim)
				{
					_filePath = pathPtr;
					return true;
				}
				if(*pathPtr == 0)
				{
					_found = true;
					return true;
				}
			}while(*pathPtr++ == *name++);
			return false;
		}
	};

	template<class FsDriverType>
	class File
	{
		FsDriverType &_driver;
	public:
		File(FsDriverType &driver)
			:_driver(driver)
		{
		
		}
	};
	
	template<class FsDriverType>
	class FileSystem
	{
		FsDriverType _driver;
	public:
		FsNode FindNode(const uint8_t *nodePath);
		bool ListDirectory(FsNode dir, DirectoryLister &directoryLister);
		
	};

	
	template<class FsDriverType>
	FsNode FileSystem<FsDriverType>::FindNode(const uint8_t *nodePath)
	{
		FindNodeLister lister(nodePath, _driver);
		FsNode node = lister.Find();
		return node;
	}
}
}
