#pragma once

#include <crc.h>
#include <noncopyable.h>

namespace Mcucpp
{
	template<class CrcClassParam, class Source>
	class CrcAdapter :public Source
	{
		typedef typename CrcClassParam::ResultType CrcType;
		Source &GetBase()
		{
			return static_cast<Source&>(*this);
		}
	public:
		typedef CrcClassParam CrcClass;

		CrcAdapter()
			:Crc(GetBase())
		{
		}
		////////////////////////////////////////////////////////////
		/// Template constructors to bypass parameters to base class
		//  constructors.
		////////////////////////////////////////////////////////////
		template<class T1>
		CrcAdapter(T1 arg1)
			:Source(arg1),
			Crc(GetBase())
		{
		}

		template<class T1, class T2>
		CrcAdapter(T1 arg1, T2 arg2)
			:Source(arg1, arg2),
			Crc(GetBase())
		{
		}

		template<class T1, class T2, class T3>
		CrcAdapter(T1 arg1, T2 arg2, T3 arg3)
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
		
		class RxTxCrc :NonCopyable
		{
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
				_rxCrc = CrcUpdate<CrcClass>(c, _rxCrc);
			}

			void UpdateTx(uint8_t c)
			{
				_txCrc = CrcUpdate<CrcClass>(c, _txCrc);
			}

			void ResetRx()
			{
				_rxCrc = CrcClass::Init;
			}
			
			void ResetTx()
			{
				_txCrc = CrcClass::Init;
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
		} Crc;
	};
}