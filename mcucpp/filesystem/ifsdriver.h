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
#include <filesystem/idirectorylister.h>

namespace Mcucpp
{
namespace Fs
{
	/// Block - mininum data size to read/write from/to block device -> sector 
	/// Chunk - file system allocation unit -> cluster 
	/// FsNode always points to block
	
	class IFsDriver
	{
	public:
		virtual ErrorCode GetError()=0;
		virtual FsNode RootDirectory()=0;
		virtual bool ListDirectory(FsNode dir, DirectoryLister &directoryLister)=0;
		virtual uint32_t GetParameter(FsParams param)=0;
		virtual uint32_t GetBlocksPerNode(FsNode node)=0;
		virtual FsNode GetNextChunk(FsNode node)=0;
		virtual FsNode AllocNextChunk(FsNode parentNode)=0;
		virtual FsNode CreateNode(FsNode parentDir, FileAttributes attributes, uint8_t *nodeName)=0;
		virtual bool ReadBlock(FsNode node, uint8_t *buffer)=0;
		virtual bool WriteBlock(FsNode node, const uint8_t *buffer)=0;
		virtual bool EndOfFile(Fs::FsNode node)=0;
		virtual bool Mount()=0;
		virtual void Unmount()=0;
	};	
}}

