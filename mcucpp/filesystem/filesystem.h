#pragma once

#include <filesystem/filesystementry.h>
#include <filesystem/ifsdriver.h>
#include <filesystem/idirectorylister.h>
#include <filesystem/findnodelister.h>
#include <filesystem/file.h>

namespace Mcucpp
{
namespace Fs
{

	class FileSystem
	{
		IFsDriver &_driver;
	public:
	    FileSystem(IFsDriver &driver)
	    :_driver(driver)
	    {

	    }

		//File OpenFile(const uint8_t *nodePath);
		bool FileExists(const uint8_t *nodePath);
		bool DirectoryExists(const uint8_t *nodePath);
		bool ListDirectory(FsNode dir, DirectoryLister &directoryLister);
	};
}
}
