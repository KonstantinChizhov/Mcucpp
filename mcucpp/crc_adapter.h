//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#include <crc.h>
#include <noncopyable.h>

namespace Mcucpp
{
	template<class Source, class CrcClassParam>
	class RxTxCrc :NonCopyable
	{
		typedef typename CrcClassParam::ResultType CrcType;
		CrcType _rxCrc;
		CrcType _txCrc;
		Source &_source;
	public:
		RxTxCrc(Source &source)
			:_source(source)
		{
			ResetRx();
			ResetTx();
		}

		void UpdateRx(uint8_t c)
		{
			_rxCrc = CrcUpdate<CrcClassParam>(c, _rxCrc);
		}

		void UpdateTx(uint8_t c)
		{
			_txCrc = CrcUpdate<CrcClassParam>(c, _txCrc);
		}

		void ResetRx()
		{
			_rxCrc = CrcClassParam::Init;
		}
		
		void ResetTx()
		{
			_txCrc = CrcClassParam::Init;
		}
		
		CrcType GetRx()
		{
			return _rxCrc;
		}
		
		CrcType GetTx()
		{
			return _txCrc;
		}
		
		bool ReadMsbAndValidate()
		{
			CrcType expectedCrc = 0;
			for(size_t i = 0; i < sizeof(CrcType); i++)
			{
				expectedCrc |= (uint8_t)_source.Read();
				expectedCrc <<= 8;
			}
			
			return expectedCrc != _rxCrc;
		}
		
		bool ReadLsbAndValidate()
		{
			CrcType expectedCrc = 0;
			for(size_t i = 0; i < sizeof(CrcType); i++)
			{
				expectedCrc |= (CrcType)_source.Read() << ((sizeof(CrcType) - 1) * 8);
				expectedCrc >>= 8;
			}	
			return expectedCrc != _rxCrc;
		}
		
		// Send accumulated CRC to communication device LSB
		void SendLsb()
		{
			CrcType crc = _txCrc;
			for(size_t i = 0; i < sizeof(CrcType); i++)
			{
				_source.Write((uint8_t)crc);
				crc >>= 8;
			}
		}
		
		// Send accumulated CRC to communication device MSB
		void SendMsb()
		{
			CrcType crc = _txCrc;
			for(size_t i = 0; i < sizeof(CrcType); i++)
			{
				_source.Write((uint8_t)(crc >> ((sizeof(CrcType) - 1) * 8)));
				crc <<= 8;
			}
		}
	};
	
	
	template<class CrcClassParam, class Source>
	class CrcMixin :public Source
	{
		typedef typename CrcClassParam::ResultType CrcType;
		Source &GetBase()
		{
			return static_cast<Source&>(*this);
		}
	public:
		typedef CrcClassParam CrcClass;

		CrcMixin()
			:Crc(GetBase())
		{
		}
		////////////////////////////////////////////////////////////
		/// Template constructors to bypass parameters to base class
		//  constructors.
		////////////////////////////////////////////////////////////
		template<class T1>
		CrcMixin(T1 arg1)
			:Source(arg1),
			Crc(GetBase())
		{
		}

		template<class T1, class T2>
		CrcMixin(T1 arg1, T2 arg2)
			:Source(arg1, arg2),
			Crc(GetBase())
		{
		}

		template<class T1, class T2, class T3>
		CrcMixin(T1 arg1, T2 arg2, T3 arg3)
			:Source(arg1, arg2, arg3),
			Crc(GetBase())
		{
		}
		
		uint8_t Read()
		{
			uint8_t c = Source::Read();
			Crc.UpdateRx(c);
			return c;
		}
		
		void Write(uint8_t c)
		{
			Crc.UpdateTx(c);
			Source::Write(c);
		}
		
		RxTxCrc<Source, CrcClassParam> Crc;
	};
	
	template<class CrcClassParam, class Source>
	class CrcAdapter
	{
		typedef typename CrcClassParam::ResultType CrcType;
		Source &_source;
	public:
		typedef CrcClassParam CrcClass;

		CrcAdapter(Source &source)
			:_source(source),
			Crc(source)
		{
		}
		
		uint8_t Read()
		{
			uint8_t c = _source.Read();
			Crc.UpdateRx(c);
			return c;
		}
		
		void Write(uint8_t c)
		{
			Crc.UpdateTx(c);
			_source.Write(c);
		}
		
		Source *operator->(){return &_source;}
		
		RxTxCrc<Source, CrcClassParam> Crc;
	};
}