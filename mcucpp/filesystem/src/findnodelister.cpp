
#include <filesystem/findnodelister.h>

namespace Mcucpp
{
namespace Fs
{
	FindNodeLister::FindNodeLister(IFsDriver &driver)
		:_filePath(nullptr),
		_driver(driver),
		_found(false),
		_pathMatched(false),
		_foundEntry(nullptr)
	{
		
	}

	bool FindNodeLister::Find(const uint8_t *filePath, FileSystemEntry *foundEntry)
	{
		if(foundEntry == nullptr || filePath == nullptr)
		{
			return false;
		}
		_filePath = filePath;
		_currentNode = _driver.RootDirectory();
		_found = false;
		_pathMatched = false;
		_foundEntry = foundEntry;
		do
		{
			if(!_driver.ListDirectory(_currentNode, *this))
			{
				return false;
			}
			if(!_pathMatched)
			{
				return false;
			}
		}while(!_found);
		return true;
	}

	bool FindNodeLister::DirectoryEntry(const FileSystemEntry &entry)
	{
		if(PathElementMatch(entry.Name()))
		{
			_currentNode = entry.Node();
			_pathMatched = true;
			*_foundEntry = entry;
			return false;
		}
		else
		{
			_pathMatched = false;
		}
		return true;
	}

	bool FindNodeLister::PathElementMatch(const uint8_t *name)
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
		
}}