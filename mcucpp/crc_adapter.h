
#include <crc.h>
namespace Mcucpp
{
	template<class CrcClass, class Source>
	class CrcAdapter :public Source
	{
		typedef typename CrcClass::ResultType CrcType;
		static CrcType _rxCrc;
		static CrcType _txCrc;
	public:
		static uint8_t Read()
		{
			uint8_t c = Source::Read();
			_rxCrc = CrcUpdate<CrcClass>(c, _rxCrc);
			return c;
		}
		
		static void Write(uint8_t c)
		{
			_txCrc = CrcUpdate<CrcClass>(c, _txCrc);
			Write(c);
		}
		
		class Crc
		{
		public:
			static void ResetRx()
			{
				_rxCrc = CrcClass::Init;
			}
			
			static void ResetTx()
			{
				_txCrc = CrcClass::Init;
			}
			
			static CrcType GetRx()
			{
				return _rxCrc;
			}
			
			static CrcType GetTx()
			{
				return _txCrc;
			}
		};
	};
	
	template<class CrcClass, class Source>
	typename CrcClass::ResultType CrcAdapter<CrcClass, Source>::_rxCrc = CrcClass::Init;
	
	template<class CrcClass, class Source>
	typename CrcClass::ResultType CrcAdapter<CrcClass, Source>::_txCrc = CrcClass::Init;
}