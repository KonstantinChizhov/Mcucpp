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
#include <template_utils.h>
#include <compiler.h>
#include <new>

namespace Mcucpp
{
namespace Fs
{
	class FileSystemEntry
	{
		mutable uint8_t *_name;
		FileAttributes _attributes;
		FsNode _node;
		FsNode _parent;
		TFileSize _size;
	public:
		FileSystemEntry()
			:_name(nullptr),
			_attributes(FsAttributeNormal),
			_node(0),
			_parent(0),
			_size(0)
		{

		}

		~FileSystemEntry()
		{
			if (_name)
			{
				delete[] _name;
			}
		}
		// move semantic, like std::auto_ptr
		// transfers buffers ownership to target object
		// TODO: use c++11 move constructor
		FileSystemEntry(const FileSystemEntry &src)
			:_name(src._name),
			_attributes(src._attributes),
			_node(src._node),
			_parent(src._parent),
			_size(src._size)
		{
			src._name = nullptr;
		}

		FileSystemEntry & operator=(const FileSystemEntry &src)
		{
			_name = src._name;
			_attributes = src._attributes;
			_node = src._node,
			_parent = src._parent;
			_size = src._size;
			src._name = nullptr;
			return *this;
		}

		const uint8_t *Name() const { return _name; }
		FileAttributes Attributes() const { return _attributes; }
		FsNode Node() const { return _node; }
		FsNode Parent () const { return _parent; }
		TFileSize Size() const {return _size; }

		void SetName(uint8_t *name, size_t nameSize, bool copy = true)
		{
			if (_name)
			{
				delete[] _name;
			}
			if (name)
			{
				if(!copy)
				{
					_name = name;
					return;
				}
				_name = new (std::nothrow) uint8_t[nameSize + 1];
				if (!_name)
				{
					return;
				}
				Util::copy(name, name + nameSize, _name);
				_name[nameSize] = 0;
			}
			else
			{
				_name = nullptr;
			}
		}

		void SetAttributes(FileAttributes attributes) { _attributes = attributes; }
		void SetNode(FsNode node) { _node = node; }
		void SetParent(FsNode parent ) { _parent = parent; }
		void SetSize(TFileSize size) { _size = size; }
	};
	
}}