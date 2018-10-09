#include <filesystem/filesystem.h>
using namespace Mcucpp::Fs;

//File FileSystem::OpenFile(const uint8_t * nodePath)
//{
//	FindNodeLister lister(_driver);
//	FileSystemEntry fileEntry;
//	lister.Find(nodePath, /*out*/ &fileEntry);
//	return File(_driver, fileEntry.Node(), fileEntry.Size());
//}

bool FileSystem::FileExists(const uint8_t *nodePath)
{
	FindNodeLister lister(_driver);
	FileSystemEntry fileEntry;
	return lister.Find(nodePath, /*out*/ &fileEntry) &&
		((fileEntry.Attributes() & FsAttributeEntryTypeMask) == FsAttributeNormal);
}

bool FileSystem::DirectoryExists(const uint8_t *nodePath)
{
	FindNodeLister lister(_driver);
	FileSystemEntry fileEntry;
	return lister.Find(nodePath, /*out*/ &fileEntry) &&
		((fileEntry.Attributes() & FsAttributeEntryTypeMask) == FsAttributeDirectory);
}

bool FileSystem::ListDirectory(FsNode dir, DirectoryLister &directoryLister)
{
    return false;
}

