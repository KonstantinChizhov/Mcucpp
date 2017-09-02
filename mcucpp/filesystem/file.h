//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2017
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#pragma once

#include <filesystem/fscommon.h>
#include <filesystem/ifsdriver.h>
#include <enum.h>

namespace Mcucpp
{
namespace Fs
{
	enum FileFlags
	{
		FileFlagsNone = 0,
		FileFlagWritable = 1,
		FileFlagBufferDirty = 2,
		FileFlagEof = 4
	};
	
	DECLARE_ENUM_OPERATIONS(FileFlags)
		
	class File
	{	
		IFsDriver &_driver;
		FsNode _firstNode;
		FsNode _current;
		uint8_t *_blockBuffer;
		FileFlags _flags;
		uint32_t _positionInFile;
		TFileSize _size;
		uint16_t _positionInBuffer;
		uint16_t _blockInChunk;
		uint16_t _blockSize;
		
	public:
		File(IFsDriver &driver, FsNode node, TFileSize size);
		~File();
		
		// move semantic, like std::auto_ptr
		// transfers buffers ownership to target object
		// TODO: use c++11 move constructor
		File(File &);
		File & operator=(File &);
		
		void Flush();
		uint8_t Read();
		size_t Read(void *buffer, size_t bytesToRead);
		bool Write(uint8_t value);
		bool Seek(uint32_t pos);
		bool EndOfFile();
	};
}}