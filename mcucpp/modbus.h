// Modbus rtu implementation

#pragma once

#include <crc.h>

#if 0
// Implement this class to work as slave
class ModbusHandler : public Mcucpp::ModbusHandlerSuperclass
{
	// Define HandleAddr, ReplyAddr and implemented functions
	static const uint8_t slaveAddr = 42;
public:
	// HandleAddr should return true if slave wants to handle request
	static bool HandleAddr( uint8_t adr ) { return adr == slaveAddr || adr == MB_ADDRESS_BROADCAST; }
	// ReplyAddr should return true if slave wants to send reply to master. Slave shouldn't answer to broadcast requests.
	static bool ReplyAddr ( uint8_t adr ) { return adr == slaveAddr; }
	//static MBExceptionEnum RegInputRead( uint8_t* buf, uint16_t addr, uint8_t nregs );
	//static MBExceptionEnum RegHoldingRead( uint8_t* buf, uint16_t addr, uint8_t nregs );
	//static MBExceptionEnum RegHoldingWrite( uint8_t* buf, uint16_t addr, uint8_t nregs );
	//static MBExceptionEnum RegDiscreteRead( uint8_t* buf, uint16_t addr, uint16_t nregs );
	//static MBExceptionEnum RegCoilsRead( uint8_t* buf, uint16_t addr, uint16_t nregs );
	//static MBExceptionEnum RegCoilsWrite( uint8_t* buf, uint16_t addr, uint16_t nregs );
};
#endif

// Uart class must have static function Uart::Timer35Restart() which triggers
// the mechanism of calling ModbusSlave<Uart,Handler>::Timer35Fired() in T3_5 us
// after last call of Uart::Timer35Restart()

// Modbus states that a baud rate higher than 19200 must use a fixed 750 us
// for inter character time out and 1.75 ms for a frame delay.
// For baud rates below 19200 the timeing is more critical and has to be calculated.
// E.g. 9600 baud in a 10 bit packet is 960 characters per second
// In milliseconds this will be 960characters per 1000ms. So for 1 character
// 1000ms/960characters is 1.04167ms per character and finaly modbus states an
// intercharacter must be 1.5T or 1.5 times longer than a normal character and thus
// 1.5T = 1.04167ms * 1.5 = 1.5625ms. A frame delay is 3.5T.
//if (baud > 19200){
//    T1_5 = 750;
//    T3_5 = 1750;
//}else{
//    T1_5 = 15000000/baud;
//    T3_5 = 35000000/baud;
//}

// Implemented functions:
// 01 (0x01) Read Coils
// 02 (0x02) Read Discrete Inputs
// 03 (0x03) Read Holding Registers
// 04 (0x04) Read Input Registers
// 05 (0x05) Write Single Coil
// 06 (0x06) Write Single Register
// 15 (0x0F) Write Multiple Coils
// 16 (0x10) Write Multiple registers

// TODO: error handlling

namespace Mcucpp
{

	class ModbusConsts
	{
	protected:
		typedef enum
		{
			MB_EX_NONE                     = 0x00,
			MB_EX_ILLEGAL_FUNCTION         = 0x01,
			MB_EX_ILLEGAL_DATA_ADDRESS     = 0x02,
			MB_EX_ILLEGAL_DATA_VALUE       = 0x03,
			MB_EX_SLAVE_DEVICE_FAILURE     = 0x04,
			MB_EX_ACKNOWLEDGE              = 0x05,
			MB_EX_SLAVE_BUSY               = 0x06,
			MB_EX_MEMORY_PARITY_ERROR      = 0x08,
			MB_EX_GATEWAY_PATH_FAILED      = 0x0A,
			MB_EX_GATEWAY_TGT_FAILED       = 0x0B
		} MBExceptionEnum;

		enum{
			MB_ADDRESS_BROADCAST = 0,      // Modbus broadcast address.
			MB_ADDRESS_MIN       = 1,      // Smallest possible slave address.
			MB_ADDRESS_MAX       = 247,    // Biggest possible slave address.
			MB_SER_PDU_SIZE_MIN  = 4,      // Minimum size of a Modbus RTU frame.
			MB_SER_PDU_SIZE_MAX  = 256,    // Maximum size of a Modbus RTU frame.
		};
		
		enum{
			MB_FUNC_NONE                          =  0 ,
			MB_FUNC_READ_COILS                    =  1 ,
			MB_FUNC_READ_DISCRETE_INPUTS          =  2 ,
			MB_FUNC_WRITE_SINGLE_COIL             =  5 ,
			MB_FUNC_WRITE_MULTIPLE_COILS          = 15 ,
			MB_FUNC_READ_HOLDING_REGISTER         =  3 ,
			MB_FUNC_READ_INPUT_REGISTER           =  4 ,
			MB_FUNC_WRITE_REGISTER                =  6 ,
			MB_FUNC_WRITE_MULTIPLE_REGISTERS      = 16 ,
			MB_FUNC_READWRITE_MULTIPLE_REGISTERS  = 23 ,
			MB_FUNC_DIAG_READ_EXCEPTION           =  7 ,
			MB_FUNC_DIAG_DIAGNOSTIC               =  8 ,
			MB_FUNC_DIAG_GET_COM_EVENT_CNT        = 11 ,
			MB_FUNC_DIAG_GET_COM_EVENT_LOG        = 12 ,
			MB_FUNC_OTHER_REPORT_SLAVEID          = 17 ,
			MB_FUNC_ERROR                         = 128 ,
		};
		
		enum{ // request/response offsets
			MB_SER_PDU_ADDR_OFF =  0,      // Offset of slave address in Ser-PDU.
			MB_PDU_FUNC_OFF     =  1,      // Offset of Modbus-PDU in Ser-PDU.
			MB_PDU_DATA_OFF     =  2,
		};
		
		enum{
			MB_PDU_FUNC_RESPONSE_LEN				= MB_PDU_DATA_OFF + 0,
			MB_PDU_FUNC_RESPONSE_DATA				= MB_PDU_DATA_OFF + 1,

			MB_PDU_FUNC_READ_ADDR_OFF               = MB_PDU_DATA_OFF + 0,
			MB_PDU_FUNC_READ_REGCNT_OFF             = MB_PDU_DATA_OFF + 2 ,
			MB_PDU_FUNC_READ_SIZE                   = 4 ,
			MB_PDU_FUNC_READ_REG_CNT_MAX             = 0x007D ,
			MB_PDU_FUNC_READ_COIL_CNT_MAX            = 0x07D0 ,
			
			MB_PDU_FUNC_WRITE_ADDR_OFF              = MB_PDU_DATA_OFF + 0,
			MB_PDU_FUNC_WRITE_VALUE_OFF             = MB_PDU_DATA_OFF + 2 ,
			MB_PDU_FUNC_WRITE_SIZE                  = 4 ,
			
			MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          = MB_PDU_DATA_OFF + 0 ,
			MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        = MB_PDU_DATA_OFF + 2 ,
			MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       = MB_PDU_DATA_OFF + 4 ,
			MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        = MB_PDU_DATA_OFF + 5 ,
			MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          = 5 ,
			MB_PDU_FUNC_WRITE_MUL_REG_CNT_MAX       = 0x007B ,
			MB_PDU_FUNC_WRITE_MUL_COIL_CNT_MAX      = 0x07B0 ,
			
			MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     = MB_PDU_DATA_OFF + 0 ,
			MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   = MB_PDU_DATA_OFF + 2 ,
			MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    = MB_PDU_DATA_OFF + 4 ,
			MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  = MB_PDU_DATA_OFF + 6 ,
			MB_PDU_FUNC_READWRITE_BYTECNT_OFF       = MB_PDU_DATA_OFF + 8 ,
			MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  = MB_PDU_DATA_OFF + 9 ,
			MB_PDU_FUNC_READWRITE_SIZE_MIN          = 9 ,
		};
	};

	class ModbusHandlerSuperclass : public ModbusConsts
	{
	public:
		static MBExceptionEnum RegInputRead   ( uint8_t* buf, uint16_t addr, uint8_t nregs )  { return MB_EX_ILLEGAL_FUNCTION; }
		static MBExceptionEnum RegHoldingRead ( uint8_t* buf, uint16_t addr, uint8_t nregs )  { return MB_EX_ILLEGAL_FUNCTION; }
		static MBExceptionEnum RegHoldingWrite( uint8_t* buf, uint16_t addr, uint8_t nregs )  { return MB_EX_ILLEGAL_FUNCTION; }
		static MBExceptionEnum RegDiscreteRead( uint8_t* buf, uint16_t addr, uint16_t nregs ) { return MB_EX_ILLEGAL_FUNCTION; }
		static MBExceptionEnum RegCoilsRead   ( uint8_t* buf, uint16_t addr, uint16_t nregs ) { return MB_EX_ILLEGAL_FUNCTION; }
		static MBExceptionEnum RegCoilsWrite  ( uint8_t* buf, uint16_t addr, uint16_t nregs ) { return MB_EX_ILLEGAL_FUNCTION; }
	};

	template<class Uart>
	class ModbusBase : public ModbusConsts
	{
	public:
		enum FsmEnumDecl{
			FSM_UNINIT,
			FSM_RECV_WAIT,
			FSM_RECV_WORK,
			FSM_RECV_DONE,
			FSM_SEND,
		};
		typedef enum FsmEnumDecl FsmEnum;
	public:
		static inline void Init()
		{
			Uart::EnableRx();
			//Uart::EnableInterrupt(Uart::RxNotEmptyInt);
		}

		static void TxHandler()
		{
			if( FSM_SEND==state ){
				if(bufPos<bufLen){
					Uart::Write(buf[bufPos++]);
				}else{
					Uart::DisableTx();
					//Uart::DisableInterrupt(Uart::TxEmptyInt);
					StateStartRecv();
				}
			}else{
				// error
				Uart::DisableTx();
				//Uart::DisableInterrupt(Uart::TxEmptyInt);
			}
		}

		static void RxHandler()
		{
			uint8_t b = Uart::Read();
			switch(state){
			case FSM_RECV_WAIT:
				state = FSM_RECV_WORK;
				// break; not need
			case FSM_RECV_WORK:
				Uart::Timer35Restart();
				if(bufPos < MB_SER_PDU_SIZE_MAX){
					buf[bufPos++] = b;
				}// else error
				break;
			default:
				// error
				break;
			}
		}
		
		static inline void Timer35Fired()
		{
			if(FSM_RECV_WORK==state){
				state = FSM_RECV_DONE;
			}// else error
		}
		
		static inline FsmEnum GetState()
		{
			return state;
		}
		
	protected:
		static void StateStartRecv()
		{
			bufPos = 0;
			state = FSM_RECV_WAIT;
		}

		static void StateStartSend()
		{
			bufPos = 0;
			state = FSM_SEND;
			Uart::EnableTx();
			//Uart::EnableInterrupt(Uart::TxEmptyInt);
		}

	protected:
		static volatile FsmEnum state;
		static volatile unsigned bufPos;
		static volatile unsigned bufLen;
		static uint8_t buf[MB_SER_PDU_SIZE_MAX];
	};

	template<class Uart>
	volatile enum ModbusBase<Uart>::FsmEnumDecl ModbusBase<Uart>::state = FSM_UNINIT;
 
	template<class Uart>
	uint8_t ModbusBase<Uart>::buf[MB_SER_PDU_SIZE_MAX];

	template<class Uart>
	volatile unsigned ModbusBase<Uart>::bufPos;

	template<class Uart>
	volatile unsigned ModbusBase<Uart>::bufLen;


	
	// Uart - uart interface class
	// Handler - ModbusHandlerSuperclass inherited class
	template<class Uart, class Handler>
	class ModbusSlave : public ModbusBase<Uart>
	{
		typedef ModbusBase<Uart> Base;
	public:
	
		static inline void Init(){
			Base::StateStartRecv();
			Base::Init();
		}

		static inline bool Poll()
		{
			// return true if received correct packet for current slave address
			if( Base::FSM_RECV_DONE == Base::state ){
				return ParseAnswer();
			}else{
				return false;
			}
		}
		
		static inline uint8_t Addr()
		{
			// return slave address from the request packet
			return Base::buf[Base::MB_SER_PDU_ADDR_OFF];
		}

	protected:
		static bool ParseAnswer()
		{
			if	(	Base::bufPos < Base::MB_SER_PDU_SIZE_MIN ||
					!Handler::HandleAddr( Base::buf[Base::MB_SER_PDU_ADDR_OFF] ) ||
					ComputeCrc<Crc16ModbusTable>(Base::buf, Base::bufPos) != 0
				)
			{
				// wrong gata/address received
				Base::StateStartRecv();
				return false;
			}
			
			ModbusConsts::MBExceptionEnum ex = Base::MB_EX_ILLEGAL_FUNCTION;
			uint16_t regAddr, regCnt;
			uint8_t anslen=0, func;
			func = Base::buf[Base::MB_PDU_FUNC_OFF];
			regAddr =  Base::buf[Base::MB_PDU_FUNC_READ_ADDR_OFF] << 8;
			regAddr |= Base::buf[Base::MB_PDU_FUNC_READ_ADDR_OFF+1];
			regCnt =  Base::buf[Base::MB_PDU_FUNC_READ_REGCNT_OFF] << 8;
			regCnt |= Base::buf[Base::MB_PDU_FUNC_READ_REGCNT_OFF+1];
			switch(func){
			case Base::MB_FUNC_READ_COILS:					// 01 (0x01) Read Coils
				if( regCnt < 1 || Base::MB_PDU_FUNC_READ_COIL_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegCoilsRead( &Base::buf[Base::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt>>3;	// regCnt>>3 == regCnt/8
					if(regCnt & 0x07){
						anslen++;
					}
				}
				break;
			case Base::MB_FUNC_READ_DISCRETE_INPUTS:		// 02 (0x02) Read Discrete Inputs
				if( regCnt < 1 || Base::MB_PDU_FUNC_READ_COIL_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegDiscreteRead( &Base::buf[Base::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt>>3;	// regCnt>>3 == regCnt/8
					if(regCnt & 0x07){
						anslen++;
					}
				}
				break;
			case Base::MB_FUNC_READ_HOLDING_REGISTER:		// 03 (0x03) Read Holding Registers
				if( regCnt < 1 || Base::MB_PDU_FUNC_READ_REG_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegHoldingRead( &Base::buf[Base::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt<<1;	// regCnt<<1 == regCnt*2
				}
				break;
			case Base::MB_FUNC_READ_INPUT_REGISTER:			// 04 (0x04) Read Input Registers
				if( regCnt < 1 || Base::MB_PDU_FUNC_READ_REG_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegInputRead( &Base::buf[Base::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt<<1;	// regCnt<<1 == regCnt*2
				}
				break;
			case Base::MB_FUNC_WRITE_SINGLE_COIL:			// 05 (0x05) Write Single Coil
				ex = Handler::RegCoilsWrite( &Base::buf[Base::MB_PDU_FUNC_WRITE_VALUE_OFF], regAddr, 1 );
				break;
			case Base::MB_FUNC_WRITE_REGISTER:				// 06 (0x06) Write Single Register
				ex = Handler::RegHoldingWrite( &Base::buf[Base::MB_PDU_FUNC_WRITE_VALUE_OFF], regAddr, 1 );
				break;
			case Base::MB_FUNC_WRITE_MULTIPLE_COILS:		// 15 (0x0F) Write Multiple Coils
				if( regCnt < 1 || Base::MB_PDU_FUNC_WRITE_MUL_COIL_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegCoilsWrite( &Base::buf[Base::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], regAddr, regCnt );
				}
				break;
			case Base::MB_FUNC_WRITE_MULTIPLE_REGISTERS:	// 16 (0x10) Write Multiple registers
				if( regCnt < 1 || Base::MB_PDU_FUNC_WRITE_MUL_REG_CNT_MAX < regCnt ){
					ex = Base::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegHoldingWrite( &Base::buf[Base::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], regAddr, regCnt );
				}
				break;
			//case Base::MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
			//default:
			//	ex=Base::MB_EX_ILLEGAL_FUNCTION;
			//	break;
			}
			
			if( !Handler::ReplyAddr( Base::buf[Base::MB_SER_PDU_ADDR_OFF] ) ){
				Base::StateStartRecv();
				return true;
			}
			
			uint16_t crc, len;
			if(ex!=Base::MB_EX_NONE){
				Base::buf[Base::MB_PDU_FUNC_OFF] = func | Base::MB_FUNC_ERROR;
				Base::buf[Base::MB_PDU_DATA_OFF] = ex;
				len = 3;
				crc = ComputeCrc<Crc16ModbusTable>(Base::buf, len);
				Base::buf[len++] = crc&0xff;
				Base::buf[len++] = crc>>8;
				Base::bufLen = len;
			}else{
				switch(func){
				case Base::MB_FUNC_READ_COILS:						// 01 (0x01) Read Coils
				case Base::MB_FUNC_READ_DISCRETE_INPUTS:			// 02 (0x02) Read Discrete Inputs
				case Base::MB_FUNC_READ_HOLDING_REGISTER:			// 03 (0x03) Read Holding Registers
				case Base::MB_FUNC_READ_INPUT_REGISTER:				// 04 (0x04) Read Input Registers
				//case Base::MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
					Base::buf[Base::MB_PDU_FUNC_RESPONSE_LEN] = anslen;
					len = static_cast<uint16_t>(anslen) + Base::MB_PDU_FUNC_RESPONSE_DATA;
					crc = ComputeCrc<Crc16ModbusTable>(Base::buf, len);
					Base::buf[len++] = crc&0xff;
					Base::buf[len++] = crc>>8;
					Base::bufLen = len;
					break;
				case Base::MB_FUNC_WRITE_SINGLE_COIL:				// 05 (0x05) Write Single Coil
				case Base::MB_FUNC_WRITE_REGISTER:					// 06 (0x06) Write Single Register
					Base::bufLen = Base::bufPos;
					break;
				case Base::MB_FUNC_WRITE_MULTIPLE_COILS:			// 15 (0x0F) Write Multiple Coils
				case Base::MB_FUNC_WRITE_MULTIPLE_REGISTERS:		// 16 (0x10) Write Multiple registers
					len = 6;
					crc = ComputeCrc<Crc16ModbusTable>(Base::buf, len);
					Base::buf[len++] = crc&0xff;
					Base::buf[len++] = crc>>8;
					Base::bufLen = len;
					break;
				}
			}

			Base::StateStartSend();
			return true;
		}
	};


	// This class do not care about slave response timeout.
	// So you should care about it in another place.
	template<class Uart>
	class ModbusMaster : public ModbusBase<Uart>
	{
		typedef ModbusBase<Uart> Base;
	public:
		typedef enum {
			RecvSucc   ,	// Received correct answer
			Sending    ,	// Request sending
			RecvWait   ,	// Waiting for answer
			Recving    ,	// At least 1 byte received
			ErrUninit  ,	// Modbus class not initialized
			ErrData    ,	// Invalid data received
			ErrModbus  ,	// Received correct answer with modbus error (MBExceptionEnum)
			ErrTimeout ,	// Not used here, just for complete enum
		} StateEnum;
		
	public:
		static inline void Init(){
			Base::bufPos = 0;
			Base::state = Base::FSM_RECV_DONE;
			Base::Init();
		}
		
		static inline void RegInputRead( uint8_t deviceAddr, uint16_t regAddr, uint8_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, Base::MB_FUNC_READ_INPUT_REGISTER );
		}

		static inline void RegHoldingRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, Base::MB_FUNC_READ_HOLDING_REGISTER );
		}
		
		static inline void RegHoldingWrite( uint8_t deviceAddr, uint16_t regAddr, uint16_t regVal ){
			SomeSingleRW( deviceAddr, regAddr, regVal, Base::MB_FUNC_WRITE_REGISTER );
		}
		
		static inline void RegDiscreteRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, Base::MB_FUNC_READ_DISCRETE_INPUTS );
		}
		
		static inline void RegCoilsRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, Base::MB_FUNC_READ_COILS );
		}
		
		static inline void RegCoilsWrite( uint8_t deviceAddr, uint16_t regAddr, uint16_t regVal ){
			SomeSingleRW( deviceAddr, regAddr, regVal, Base::MB_FUNC_WRITE_SINGLE_COIL );
		}
		
		
		// Use this function to put data into buffer, before RegHoldingWriteMultiple or RegCoilsWriteMultiple call
		static inline uint8_t* RegWriteMultiplePrepare(){
			return &Base::buf[Base::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF];
		}

		static inline void RegHoldingWriteMultiple( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeMultipleRW( deviceAddr, regAddr, regCnt, regCnt<<1, Base::MB_FUNC_WRITE_MULTIPLE_REGISTERS );
		}

		static inline void RegCoilsWriteMultiple( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeMultipleRW( deviceAddr, regAddr, regCnt, regCnt>>3, Base::MB_FUNC_WRITE_MULTIPLE_COILS );
		}

		static StateEnum CheckState(){
			switch( Base::state ){
			case Base::FSM_UNINIT:
				return ErrUninit;
			case Base::FSM_SEND:
				return Sending;
			case Base::FSM_RECV_WAIT:
				return RecvWait;
			case Base::FSM_RECV_WORK:
				if(Base::bufPos < Base::MB_SER_PDU_SIZE_MAX){
					return Recving;
				}else{
					return ErrData;
				}
			case Base::FSM_RECV_DONE:
				if( 	Base::bufPos < Base::MB_SER_PDU_SIZE_MIN 
						|| reqAddr != Base::buf[Base::MB_SER_PDU_ADDR_OFF]
						|| reqFunc != (Base::buf[Base::MB_PDU_FUNC_OFF] & ~Base::MB_FUNC_ERROR)
						|| ComputeCrc<Crc16ModbusTable>(Base::buf, Base::bufPos) != 0 )
				{
					return ErrData;
				}else if( Base::buf[Base::MB_PDU_FUNC_OFF] >= Base::MB_FUNC_ERROR ){
					return ErrModbus;
				}else{
					return RecvSucc;
				}
			}
			return ErrUninit;
		}
		
		static ModbusConsts::MBExceptionEnum GetModbusErr(){
			return static_cast<ModbusConsts::MBExceptionEnum>(Base::buf[Base::MB_PDU_DATA_OFF]);
		}

		static uint8_t* ReceivedDataGet(){
			return &Base::buf[Base::MB_PDU_FUNC_RESPONSE_DATA];
		}
		

	
	protected:
		// similar functions batch:
		// 01 (0x01) Read Coils
		// 02 (0x02) Read Discrete Inputs
		// 03 (0x03) Read Holding Registers
		// 04 (0x04) Read Input Registers
		// 05 (0x05) Write Single Coil
		// 06 (0x06) Write Single Register
		static void SomeSingleRW( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCntOrVal , uint8_t func){
			reqAddr = deviceAddr;
			reqFunc = func;
			unsigned len = 0;
			Base::buf[len++] = deviceAddr;
			Base::buf[len++] = func;
			Base::buf[len++] = regAddr>>8;
			Base::buf[len++] = regAddr&0xff;
			Base::buf[len++] = regCntOrVal>>8;
			Base::buf[len++] = regCntOrVal&0xff;
			uint16_t crc = ComputeCrc<Crc16ModbusTable>(Base::buf, len);
			Base::buf[len++] = crc&0xff;
			Base::buf[len++] = crc>>8;
			Base::bufLen = len;
			Base::StateStartSend();
		}
		// similar functions batch:
		// 15 (0x0F) Write Multiple Coils
		// 16 (0x10) Write Multiple registers
		static void SomeMultipleRW( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt, uint8_t byteCnt , uint8_t func){
			reqAddr = deviceAddr;
			reqFunc = func;
			unsigned len = 0;
			Base::buf[len++] = deviceAddr;
			Base::buf[len++] = func;
			Base::buf[len++] = regAddr>>8;
			Base::buf[len++] = regAddr&0xff;
			Base::buf[len++] = regCnt>>8;
			Base::buf[len++] = regCnt&0xff;
			Base::buf[len++] = byteCnt;
			len += byteCnt;
			uint16_t crc = ComputeCrc<Crc16ModbusTable>(Base::buf, len);
			Base::buf[len++] = crc&0xff;
			Base::buf[len++] = crc>>8;
			Base::bufLen = len;
			Base::StateStartSend();
		}
		
	protected:
		static uint8_t reqAddr;
		static uint8_t reqFunc;
	};
	template<class Uart>
	uint8_t ModbusMaster<Uart>::reqAddr;
	template<class Uart>
	uint8_t ModbusMaster<Uart>::reqFunc;

}

