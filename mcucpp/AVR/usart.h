#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ioreg.h"
#include "ring_buffer.h"



#ifdef URSEL
enum{ursel = 1 << URSEL};
#else
enum{ursel = 0};
#endif

#define DECLARE_HW_USART(ClassName, UDR_Reg, UCSRA_Reg, UCSRB_Reg, UCSRC_Reg, UBRRL_Reg, UBRRH_Reg)\
struct ClassName\
{\
	IO_REG_WRAPPER(UDR_Reg, Udr, uint8_t);\
	IO_REG_WRAPPER(UCSRA_Reg, Ucsra, uint8_t);\
	IO_REG_WRAPPER(UCSRB_Reg, Ucsrb, uint8_t);\
	IO_REG_WRAPPER(UCSRC_Reg, Ucsrc, uint8_t);\
	IO_REG_WRAPPER(UBRRL_Reg, Ubrrl, uint8_t);\
	IO_REG_WRAPPER(UBRRH_Reg, Ubrrh, uint8_t);\
};

#ifdef UDR //the one usart
DECLARE_HW_USART(Usart0Regs, UDR, UCSRA, UCSRB, UCSRC, UBRRL, UBRRH)
#endif

#ifdef UDR0 //first usart
DECLARE_HW_USART(Usart0Regs, UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0L, UBRR0H)
#endif

#ifdef UDR1 //second usart
DECLARE_HW_USART(Usart1Regs, UDR1, UCSR1A, UCSR1B, UCSR1C, UBRR1L, UBRR1H)
#endif

template<class Regs>
class UsartBase
{
	public:
		template<unsigned long baund>
	static inline void SetBaundRate()
	{
		const unsigned int ubrr = (F_CPU/16/baund-1);
		const unsigned int ubrr2x 	(F_CPU/8/baund-1);
		const unsigned long rbaund = (F_CPU/16/(ubrr+1));	
 		const unsigned long rbaund2x (F_CPU/8/(ubrr2x+1));

		unsigned long err1;
		if(baund > rbaund)
			err1 = (baund - rbaund)*1000/baund;
		else
			err1 = (rbaund - baund)*1000/rbaund;

		unsigned long err2;
		if(baund > rbaund2x)
			err2 = (baund - rbaund2x)*1000/baund;
		else
			err2 = (rbaund2x - baund)*1000/rbaund2x;

		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			Regs::Ucsra::Set(1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			Regs::Ucsra::Set(0x00);
			ubrrToUse = ubrr;
		}
		Regs::Ubrrl::Set(ubrrToUse);
		Regs::Ubrrh::Set(ubrrToUse>>8);
	}

	static inline void SetBaundRate(unsigned long baund)
	{
		unsigned int ubrr = (F_CPU/16/baund-1);
		unsigned int ubrr2x 	(F_CPU/8/baund-1);
		unsigned long rbaund = (F_CPU/16/(ubrr+1));	
 		unsigned long rbaund2x (F_CPU/8/(ubrr2x+1));

		unsigned long err1;
		if(baund > rbaund)
			err1 = (baund - rbaund)*1000/baund;
		else
			err1 = (rbaund - baund)*1000/rbaund;

		unsigned long err2;
		if(baund > rbaund2x)
			err2 = (baund - rbaund2x)*1000/baund;
		else
			err2 = (rbaund2x - baund)*1000/rbaund2x;

		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			Regs::Ucsra::Set(1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			Regs::Ucsra::Set(0x00);
			ubrrToUse = ubrr;
		}
		Regs::Ubrrl::Set(ubrrToUse);
		Regs::Ubrrh::Set(ubrrToUse>>8);
	}

	static void Disable()
	{
		Regs::Ucsra::Set(0);
		Regs::Ucsrb::Set(0);
		Regs::Ucsrc::Set(ursel | 0);
		Regs::Ubrrl::Set(0);
		Regs::Ubrrh::Set(0);
	}

};

template<class Regs=Usart0Regs>
class PollUsart :public UsartBase<Regs>
{
	public:
	static inline void EnableTxRx()
	{
		Regs::Ucsrb::Set(0x00); 
		Regs::Ucsrc::Set(ursel | (1 << UCSZ1) | (1 << UCSZ0));
		Regs::Ucsrb::Set( (0 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN) | (1 << TXEN));
	}

	template<unsigned long baund>
	static inline void Init()
	{
		UsartBase<Regs>:: template SetBaundRate<baund>();
		EnableTxRx();
	}

	static inline void Init(unsigned long baund)
	{
		UsartBase<Regs>:: template SetBaundRate(baund);
		EnableTxRx();
	}

	static bool Putch(uint8_t c)
	{
		if(!Regs::Ucsra::template BitIsSet<(UDRE)>() )
			return false;
		Regs::Udr::Set(c);
		return true;
	}

	static bool Getch(uint8_t &c)
	{
		if(!Regs::Ucsra::template BitIsSet<(RXC)>() )
			return false;
		c = Regs::Udr::Get();
		return true;
	}

	static uint8_t BytesRecived()
	{
		return Regs::Ucsra::template BitIsSet<(RXC)>() ? 1 : 0;
	}
};


template<int TxSize, int RxSize, class Regs=Usart0Regs>
class Usart :public UsartBase<Regs>
{
public:

	static inline void EnableTxRx()
	{
		Regs::Ucsrb::Set(0x00); 
		Regs::Ucsrc::Set(ursel | (1 << UCSZ1) | (1 << UCSZ0));
		Regs::Ucsrb::Set( (1 << RXCIE) | (0 << TXCIE) | (1 << UDRIE) | (1 << RXEN) | (1 << TXEN));
	}

	template<unsigned long baund>
	static inline void Init()
	{
		UsartBase<Regs>:: template SetBaundRate<baund>();
		EnableTxRx();
	}

	static inline void Init(unsigned long baund)
	{
		UsartBase<Regs>:: template SetBaundRate(baund);
		EnableTxRx();
	}

	static bool Putch(uint8_t c)
	{
		if(_tx.IsEmpty())
		{
			while(!Regs::Ucsra::template BitIsSet<(UDRE)>() );
			Regs::Udr::Set(c);
			Regs::Ucsrb::Or(1 << UDRIE);
			return true;
		}else 
		return _tx.Write(c);
	}

	static bool Getch(uint8_t &c)
	{
		return _rx.Read(c);
	}

	static inline void TxHandler()
	{
		uint8_t c;
		if(_tx.Read(c))
			Regs::Udr::Set(c);
		else
			Regs::Ucsrb::And( ~(1 << UDRIE) );
	}

	static inline void RxHandler()
	{
		if(!_rx.Write(Regs::Udr::Get() ))//buffer overlow
		{
			//TODO: error handling
			_rx.Clear();
		}	
	}

	static void DropBuffers()
	{
		_rx.Clear();
	}

	static uint8_t BytesRecived()
	{
		return _rx.Count();
	}

	static void Disable()
	{
		
		UsartBase<Regs>::Disable();
		_rx.Clear();
		_tx.Clear();
	}


private:
	static RingBuffer<RxSize> _rx;
	static RingBuffer<TxSize> _tx;
};

template<int TxSize, int RxSize, class Regs>
	RingBuffer<RxSize> Usart<TxSize, RxSize, Regs>::_rx;
template<int TxSize, int RxSize, class Regs>
	RingBuffer<TxSize> Usart<TxSize, RxSize, Regs>::_tx;



#endif
