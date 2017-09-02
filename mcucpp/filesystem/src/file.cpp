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
#include <template_utils.h>
#include <filesystem/fscommon.h>
#include <filesystem/ifsdriver.h>
#include <filesystem/file.h>
#include <new>
#include <memory.h>

namespace Mcucpp
{
namespace Fs
{
	File::File(IFsDriver &driver, FsNode node, TFileSize size)
		:_driver(driver),
		_firstNode(node),
		_current(0),
		_positionInFile(0),
		_flags(FileFlagsNone),
		_blockInChunk(0),
		_size(size)
	{
		_blockSize = _driver.GetParameter(BlockSize);
		_blockBuffer = new (std::nothrow) uint8_t [_blockSize];
		_positionInBuffer = _blockSize;
		if(!_blockBuffer)
		{
			_flags |= FileFlagEof;
		}
	}
		
	File::~File()
	{
		Flush();
		if(_blockBuffer)
		{
			delete[] _blockBuffer;
		}
	}
	
	File::File(File &moveFromFile)
		:_driver(moveFromFile._driver),
		_firstNode(moveFromFile._firstNode),
		_current(moveFromFile._current),
		_positionInFile(moveFromFile._positionInFile),
		_positionInBuffer(moveFromFile._positionInBuffer),
		_blockInChunk(moveFromFile._blockInChunk),
		_flags(moveFromFile._flags),
		_blockBuffer(moveFromFile._blockBuffer),
		_blockSize(moveFromFile._blockSize)
	{
		moveFromFile._blockBuffer = nullptr;
		moveFromFile._firstNode = EndOfFileNode;
		moveFromFile._current = EndOfFileNode;
		moveFromFile._positionInFile = 0;
		moveFromFile._positionInBuffer = _blockSize;
		moveFromFile._blockInChunk = 0;
		moveFromFile._flags = FileFlagsNone;
	}
	/*
	File::File & operator=(File &)
	{
		_firstNode = moveFromFile._firstNode;
		_current = moveFromFile._current;
		_positionInFile = moveFromFile._positionInFile;
		_positionInBuffer = moveFromFile._positionInBuffer;
		_blockInChunk = moveFromFile._blockInChunk;
		_flags = moveFromFile._flags;
		_blockBuffer = moveFromFile._blockBuffer;

		moveFromFile._blockBuffer = nullptr;
		moveFromFile._firstNode = EndOfFileNode;
		moveFromFile._current = EndOfFileNode;
		moveFromFile._positionInFile = 0;
		moveFromFile._positionInBuffer = 0;
		moveFromFile._blockInChunk = 0;
		moveFromFile._flags = FileFlagsNone;
	}*/
	
	void File::Flush()
	{
		if(!_blockBuffer)
		{
			return;
		}
		if(_current && 
			_driver.EndOfFile(_current) && 
			(_flags & FileFlagBufferDirty) && 
			(_flags & FileFlagWritable))
		{
			_driver.WriteBlock(_current + _blockInChunk, _blockBuffer);
		}
	}
	
	uint8_t File::Read()
	{
		if(_positionInBuffer >= _blockSize)
		{
			if(!_blockBuffer)
			{
				return 0;
			}
			if(!_current)
			{
				_current = _firstNode;
				_positionInFile = 0;
			}
			else
			{
				if(_driver.EndOfFile(_current))
				{
					return 0;
				}
				Flush();
				_blockInChunk++;
				if(_blockInChunk >= _driver.GetBlocksPerNode(_current))
				{
					Fs::FsNode next = _driver.GetNextChunk(_current);
					_blockInChunk = 0;
					if(_driver.EndOfFile(next))
					{
						_flags |= FileFlagEof;
						return 0;
					}
					_flags &= ~FileFlagEof;
					_current = next;
				}
				_positionInFile += _blockSize;
			}
			_driver.ReadBlock(_current + _blockInChunk, _blockBuffer);
			_positionInBuffer = 0;
		}
		
		uint8_t res = _blockBuffer[_positionInBuffer++];
		if(_positionInBuffer + _positionInFile >= _size)
		{
			_flags |= FileFlagEof;
		}
		return res;
	}
	
	size_t File::Read(void *buffer, size_t bytesToRead)
	{
		if(!_blockBuffer)
		{
			return 0;
		}
		
		uint32_t totalBytesAvailable = 0;
		if(_positionInBuffer + _positionInFile < _size)
		{
			totalBytesAvailable = _size - (_positionInBuffer + _positionInFile);
		}
		uint8_t *dest = static_cast<uint8_t *>(buffer);
		size_t bytesInBuffer = _blockSize - _positionInBuffer;
		size_t bytesRead = Util::min(bytesToRead, bytesInBuffer);
		if(bytesRead > totalBytesAvailable)
		{
			bytesRead = totalBytesAvailable;
		}
		
		memcpy(dest, &_blockBuffer[_positionInBuffer] , bytesRead);
		_positionInBuffer += bytesRead;
		
		while(bytesRead < bytesToRead && 
			_positionInBuffer + _positionInFile < _size)
		{
			if(!_current)
			{
				_current = _firstNode;
				_positionInFile = 0;
			}
			else
			{
				if(_driver.EndOfFile(_current))
				{
					_flags |= FileFlagEof;
					break;
				}
				Flush();
				_blockInChunk++;
				if(_blockInChunk >= _driver.GetBlocksPerNode(_current))
				{
					Fs::FsNode next = _driver.GetNextChunk(_current);
					_blockInChunk = 0;
					if(_driver.EndOfFile(next))
					{
						_flags |= FileFlagEof;
						break;
					}
					_flags &= ~FileFlagEof;
					_current = next;
				}
				_positionInFile += _blockSize;
			}
			_driver.ReadBlock(_current + _blockInChunk, _blockBuffer);
			totalBytesAvailable = 0;
			if (_positionInFile < _size)
			{
				totalBytesAvailable = _size - (_positionInFile);
			}
			size_t chunkSize = Util::min(bytesToRead - bytesRead, (size_t)_blockSize);
			if (chunkSize > totalBytesAvailable)
			{
				chunkSize = totalBytesAvailable;
				_flags |= FileFlagEof;
			}
			_positionInBuffer = chunkSize;
			dest = static_cast<uint8_t *>(buffer) + bytesRead;
			memcpy(dest, _blockBuffer, chunkSize);
			bytesRead += chunkSize;
		}
		
		if(_positionInBuffer + _positionInFile >= _size)
		{
			_flags |= FileFlagEof;
		}
		return bytesRead;
	}
	
	bool File::Write(uint8_t value)
	{
		return false;
	}
	
	bool File::Seek(uint32_t pos)
	{	
		if(!_blockBuffer)
		{
			return false;
		}
		if(pos >= _positionInFile + _blockSize || pos < _positionInFile)
		{
			if(pos < _positionInFile)
			{
				_current = _firstNode;
				_positionInFile = 0;
			}
			Flush();
		
			while(pos > _positionInFile + _blockSize )
			{
				if(_driver.EndOfFile(_current))
				{
					return false;
				}
				if(_blockInChunk >= _driver.GetBlocksPerNode(_current))
				{
					_current = _driver.GetNextChunk(_current);
					_blockInChunk = 0;
				}
				if(_driver.EndOfFile(_current))
				{
					return false;
				}
				_positionInFile += _blockSize;
			}
			_driver.ReadBlock(_current + _blockInChunk, _blockBuffer);
		}
		_positionInBuffer = pos - _positionInFile;
		_flags &= ~FileFlagEof;
		return false;
	}
	
	bool File::EndOfFile()
	{
		return _flags & FileFlagEof;
	}

}}