namespace Mcucpp
{
	namespace IO
	{
		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Set()
		{
			Set(true);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Set(bool val)
		{
			if(val)
				PORT::template Set<1 << PIN>();
			else
				PORT::template Clear<1 << PIN>();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDir(uint8_t val)
		{
			if(val)
				SetDirWrite();
			else SetDirRead();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Clear()
		{
			Set(false);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::Toggle()
		{
			PORT::Toggle(1 << PIN);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDirRead()
		{
			PORT::template SetPinConfiguration<PIN>( PORT::In);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetDirWrite()
		{
			ConfigPort:: template SetPinConfiguration<PIN>(PORT::Out);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::SetConfiguration(Configuration configuration)
		{
			ConfigPort:: template SetPinConfiguration<PIN>(configuration);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		bool TPin<PORT, PIN, CONFIG_PORT>::IsSet()
		{
			return PORT::PinRead() & (typename PORT::DataT)(1 << PIN);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::WaiteForSet()
		{
			while(IsSet()==0){}
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void TPin<PORT, PIN, CONFIG_PORT>::WaiteForClear()
		{
			while(IsSet()){}
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Set(bool val)
		{
			if(val)
				PORT::template Clear<1 << PIN>();
			else
				PORT::template Set<1 << PIN>();
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Set()
		{
			Set(true);
		}

		template<class PORT, uint8_t PIN, class CONFIG_PORT>
		void InvertedPin<PORT, PIN, CONFIG_PORT>::Clear()
		{
			Set(false);
		}
	}
}
