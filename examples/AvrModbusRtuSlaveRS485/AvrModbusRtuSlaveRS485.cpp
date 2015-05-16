#include <avr/interrupt.h>

#include <iopins.h>
#include <pinlist.h>
#include <usart.h>
#include <timers.h>
#include <modbus.h>
using namespace Mcucpp::IO;
using namespace Mcucpp::Timers;

typedef Mcucpp::Usart<Mcucpp::Usart0Regs> Uart;
typedef Pe2 UartTxEn; // rs485 direction pin
// Pe1 - txd pin
typedef PinList<Pe1,UartTxEn> UartOutPins;

class ModbusHandler : public Mcucpp::ModbusHandlerSuperclass
{
	// Define slaveAddr and implemented functions
	static uint8_t slaveAddr;
public:
	// HandleAddr should return true if slave wants to handle request
	static bool HandleAddr( uint8_t adr ) { return adr == slaveAddr || adr == MB_ADDRESS_BROADCAST; }
	// ReplyAddr should return true if slave wants to send reply to master. Slave shouldn't answer to broadcast requests.
	static bool ReplyAddr ( uint8_t adr ) { return adr == slaveAddr; }

	//static MBExceptionEnum RegInputRead( uint8_t* buf, uint16_t addr, uint8_t nregs );
	static MBExceptionEnum RegHoldingRead( uint8_t* buf, uint16_t addr, uint8_t nregs );
	static MBExceptionEnum RegHoldingWrite( uint8_t* buf, uint16_t addr, uint8_t nregs );
	//static MBExceptionEnum RegDiscreteRead( uint8_t* buf, uint16_t addr, uint16_t nregs );
	//static MBExceptionEnum RegCoilsRead( uint8_t* buf, uint16_t addr, uint16_t nregs );
	//static MBExceptionEnum RegCoilsWrite( uint8_t* buf, uint16_t addr, uint16_t nregs );
};
uint8_t ModbusHandler::slaveAddr = 42;

class UartWrapper : public Uart
{
public:
	inline static void Timer35Restart(){
		Timer0::Set(1);
		Timer0::Start(Timer0::Div256);
	}
	static void EnableRx(){
		UartTxEn::Clear();
		Uart::EnableInterrupt(Uart::RxNotEmptyInt);
	}
	static void EnableTx(){
		UartTxEn::Set();
		Uart::EnableInterrupt(Uart::TxEmptyInt);
	}
	static void DisableTx(){
		Uart::DisableInterrupt(Uart::TxEmptyInt);
	}
};

typedef Mcucpp::ModbusSlave<UartWrapper,ModbusHandler> Modbus;

uint16_t testReg0 = 0xdead, testReg1 = 0xbabe;




ISR(TIMER0_COMP_vect){		// Timer/Counter0 Compare Match
	Modbus::Timer35Fired();
	Timer0::Stop();
}

ISR(USART0_RX_vect){		// USART0, Rx Complete
	Modbus::RxHandler();
}

ISR(USART0_UDRE_vect){		// USART0, Tx Data Register Empty
	Modbus::TxHandler();
}

ISR(USART0_TX_vect){		// USART0, Tx Complete
	UartTxEn::Clear();
}




int main()
{
	UartOutPins::SetConfiguration<UartOutPins::Out>();

	Uart::Init<9600>();
	Uart::EnableInterrupt(Uart::TxCompleteInt);
	
	// frame delay timer
	Timer0::SetMode(Timer0::ClearOnMatch);
	Timer0::OutputCompare<0>::Set(F_CPU/256*35/9600 +1); // F_CPU*3.5*10/Baudrate/Div256
	Timer0::OutputCompare<0>::EnableInterrupt();

	Modbus::Init();

	sei();

	while(true)
	{
		Modbus::Poll();
	}

	return 0;
}


ModbusHandler::MBExceptionEnum ModbusHandler::RegHoldingRead( uint8_t* buf, uint16_t addr, uint8_t nregs )
{
	uint16_t word;
	do{
		switch(addr++){
		case 0:
			word = testReg0;
			break;
		case 1:
			word = testReg1;
			break;
		default:
			return MB_EX_ILLEGAL_DATA_ADDRESS;
		}
		*buf++ = word>>8;
		*buf++ = word&0xff;
	}while(--nregs);
	return MB_EX_NONE;
}


ModbusHandler::MBExceptionEnum ModbusHandler::RegHoldingWrite( uint8_t* buf, uint16_t addr, uint8_t nregs )
{
	uint16_t word;
	do{
		word = static_cast<uint16_t>(*buf++)<<8;
		word |= *buf++;
		switch(addr++){
		case 0:
			testReg0 = word;
			break;
		case 1:
			testReg1 = word;
			break;
		default:
			return MB_EX_ILLEGAL_DATA_ADDRESS;
		}
	}while(--nregs);
	return MB_EX_NONE;
}
