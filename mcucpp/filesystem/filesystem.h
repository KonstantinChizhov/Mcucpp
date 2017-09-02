#pragma once

#include <filesystem/filesystementry.h>
#include <filesystem/ifsdriver.h>
#include <filesystem/idirectorylister.h>
#include <filesystem/findnodelister.h>

namespace Mcucpp
{
namespace Fs
{
	
	class FileSystem
	{
		IFsDriver &_driver;
	public:
		FsNode FindNode(const uint8_t *nodePath);
		bool ListDirectory(FsNode dir, DirectoryLister &directoryLister);
	};
}
}
