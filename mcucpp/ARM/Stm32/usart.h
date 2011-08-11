
#pragma once
#include <clock.h>

namespace HAL
{
	class UsartBase
	{
		public:
		enum UsartMode
		{
			DataBits8 = 0,
			DataBits9 = USART_CR1_M,
		
			NoneParity = 0,
			EvenParity = USART_CR1_PCE,
			OddParity  = USART_CR1_PS | USART_CR1_PCE,
		
			NoClock = 0,
			
			Disabled = 0,
			RxEnable = USART_CR1_RE,
			TxEnable = USART_CR1_TE,
			RxTxEnable  = USART_CR1_RE | USART_CR1_TE,
			Default = RxTxEnable,
			
			OneStopBit         = 0,
			HalfStopBit        = USART_CR2_STOP_0,
			TwoStopBits        = USART_CR2_STOP_1,
			OneAndHalfStopBits = (USART_CR2_STOP_0 | USART_CR2_STOP_1)
		};
		
		static const int EOF = -1;
	};
	
	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<int>(left) | static_cast<int>(right));	}
		
	
	namespace Private
	{
		template<class Sr, class Dr, class Brr, class Cr1, class Cr2, class Cr3, class ClockCtrl>
		class Usart :public UsartBase
		{
			public:
			static void Init(unsigned baund, UsartMode flags = Default)
			{
				ClockCtrl::Enable();
				unsigned brr = Clock::SysClock::FPeriph() / baund;
				Brr::Set(brr);
				Cr1::Set((flags & 0xffff) | USART_CR1_UE );
				Cr2::Set((flags >> 16) & 0xffff);
			}
			
			static bool Putch(uint8_t c)
			{
				while((Sr::Get() & USART_SR_TXE) == 0);
				Dr::Set(c);
				return true;
			}

			static int Getch()
			{
				if(Sr::Get() & USART_SR_RXNE)
					return Dr::Get();
				return EOF;
			}
			
			static bool Write(uint8_t c)
			{
				return Putch();
			}
			
			static bool TxReady()
			{
				return (Sr::Get() & USART_SR_TXE) == 0;
			}
			
			static bool RxReady()
			{
				return Sr::Get() & USART_SR_RXNE;
			}
			
			class Dma
			{
				public:
				//typedef Dma1Channel4 ChannelTx;
				static void EnableTx()
				{
					
				}
				
			};
		};
	}
	
#define DECLARE_USART(SR, DR, BRR, CR1, CR2, CR3, CLOCK, className) \
	namespace Private \
	{\
		IO_REG_WRAPPER(SR, className ## Sr, uint32_t);\
		IO_REG_WRAPPER(DR, className ## Dr, uint32_t);\
		IO_REG_WRAPPER(BRR, className ## Brr, uint32_t);\
		IO_REG_WRAPPER(CR1, className ## Cr1, uint32_t);\
		IO_REG_WRAPPER(CR2, className ## Cr2, uint32_t);\
		IO_REG_WRAPPER(CR3, className ## Cr3, uint32_t);\
	}\
	typedef Private::Usart<\
		Private::className ## Sr, \
		Private::className ## Dr, \
		Private::className ## Brr,\
		Private::className ## Cr1,\
		Private::className ## Cr2,\
		Private::className ## Cr3,\
		CLOCK\
		> className;
		
		DECLARE_USART(USART1->SR, USART1->DR, USART1->BRR, USART1->CR1, USART1->CR2, USART1->CR3, Clock::Usart1Clock, Usart1);
}