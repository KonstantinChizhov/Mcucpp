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
#include <compiler.h>
#include <stdint.h>

namespace Mcucpp
{
namespace Fs
{
	typedef uint32_t FsNode;
	typedef uint32_t TFileSize;

	const FsNode EndOfFileNode = 0xffffffff;
	const uint8_t PathDelim = '/';
	const size_t MaxPath = 260;
	const size_t DefaultBlockSize = 512;

	enum FileAttributes
	{
		FsAttributeNormal = 0,
		FsAttributeReadOnly = 1,
		FsAttributeHidden = 2,
		FsAttributeSystem = 4,
		FsAttributeVolumeId = 8,
		FsAttributeDirectory = 16,
		FsAttributeArchive = 32,
		FsAttributeDevice = 64
	};

	enum FsParams
	{
		BlockSize,
		ChunkSize,
		BlocksInChunk,
		TotalBlocks,
		UsedBlocks
	};
	
	enum FsSeekMode
	{
		FsSeekBegin,
		FsSeekEnd,
		FsSeekRelative
	};

	enum ErrorCode
	{
		ErrOK,
		ErrInvalidNode = 1,
		ErrInvalidDirectory =2,
		ErrInvalidFile = 4,
		ErrDeviceNotReady = 8,
		ErrIoFailed = 16,
		ErrInvalidFs = 32,
		ErrOutOfMemory = 64
	};
	
}}