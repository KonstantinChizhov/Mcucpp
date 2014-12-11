// Modbus rtu implementation

#pragma once

#include <crc.h>

#if 0
// Implement this class to work as slave
class ModbusHandler : public Mcucpp::ModbusHandlerSuperclass
{
public:
	// Define slaveAddr and implemented functions
	static const uint8_t slaveAddr = 42;
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
		static inline void Init()
		{
			Uart::EnableRx();
			//Uart::EnableInterrupt(Uart::RxNotEmptyInt);
		}

		static void TxHandler()
		{
			if( ST_SEND==state ){
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
			case ST_RECV_WAIT:
				state = ST_RECV_WORK;
				// break; not need
			case ST_RECV_WORK:
				Uart::Timer35Restart();
				if(bufPos < MB_SER_PDU_SIZE_MAX){
					buf[bufPos++] = b;
				}
				break;
			default:
				// error
				break;
			}
		}
		
		static inline void Timer35Fired()
		{
			if(ST_RECV_WORK==state){
				state = ST_RECV_DONE;
			}// else error
		}
		
	protected:
		static void StateStartRecv()
		{
			bufPos = 0;
			state = ST_RECV_WAIT;
		}

		static void StateStartSend()
		{
			ModbusBase<Uart>::bufPos = 0;
			ModbusBase<Uart>::state = ModbusBase<Uart>::ST_SEND;
			Uart::EnableTx();
			//Uart::EnableInterrupt(Uart::TxEmptyInt);
		}

	protected:
		enum StateEnum{
			ST_UNINIT,
			ST_RECV_WAIT,
			ST_RECV_WORK,
			ST_RECV_DONE,
			ST_SEND,
		};
		static volatile enum StateEnum state;
		static volatile unsigned bufPos;
		static volatile unsigned bufLen;
		static uint8_t buf[MB_SER_PDU_SIZE_MAX];
	};
	template<class Uart>
	volatile enum ModbusBase<Uart>::StateEnum ModbusBase<Uart>::state = ST_UNINIT;

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
	public:
	
		static inline void Init(){
			ModbusBase<Uart>::StateStartRecv();
			ModbusBase<Uart>::Init();
		}

		static inline bool Poll()
		{
			// return true if received correct packet for current slave address
			if( ModbusBase<Uart>::ST_RECV_DONE == ModbusBase<Uart>::state ){
				return ParseAnswer();
			}else{
				return false;
			}
		}

	protected:
		static bool ParseAnswer()
		{
			if	(	ModbusBase<Uart>::bufPos < ModbusConsts::MB_SER_PDU_SIZE_MIN ||
					( Handler::slaveAddr                 != ModbusBase<Uart>::buf[ModbusConsts::MB_SER_PDU_ADDR_OFF] &&
					  ModbusConsts::MB_ADDRESS_BROADCAST != ModbusBase<Uart>::buf[ModbusConsts::MB_SER_PDU_ADDR_OFF]
					) ||
					ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, ModbusBase<Uart>::bufPos) != 0
				)
			{
				// wrong gata/address received
				ModbusBase<Uart>::StateStartRecv();
				return false;
			}
			
			ModbusConsts::MBExceptionEnum ex = ModbusConsts::MB_EX_ILLEGAL_FUNCTION;
			uint16_t regAddr, regCnt;
			uint8_t anslen=0, func;
			func = ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_OFF];
			regAddr =  ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_READ_ADDR_OFF] << 8;
			regAddr |= ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_READ_ADDR_OFF+1];
			regCnt =  ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_READ_REGCNT_OFF] << 8;
			regCnt |= ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_READ_REGCNT_OFF+1];
			switch(func){
			case ModbusConsts::MB_FUNC_READ_COILS:					// 01 (0x01) Read Coils
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_READ_COIL_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegCoilsRead( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt>>3;	// regCnt>>3 == regCnt/8
					if(regCnt & 0x07){
						anslen++;
					}
				}
				break;
			case ModbusConsts::MB_FUNC_READ_DISCRETE_INPUTS:		// 02 (0x02) Read Discrete Inputs
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_READ_COIL_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegDiscreteRead( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt>>3;	// regCnt>>3 == regCnt/8
					if(regCnt & 0x07){
						anslen++;
					}
				}
				break;
			case ModbusConsts::MB_FUNC_READ_HOLDING_REGISTER:		// 03 (0x03) Read Holding Registers
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_READ_REG_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegHoldingRead( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt<<1;	// regCnt<<1 == regCnt*2
				}
				break;
			case ModbusConsts::MB_FUNC_READ_INPUT_REGISTER:			// 04 (0x04) Read Input Registers
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_READ_REG_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegInputRead( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA], regAddr, regCnt );
					anslen = regCnt<<1;	// regCnt<<1 == regCnt*2
				}
				break;
			case ModbusConsts::MB_FUNC_WRITE_SINGLE_COIL:			// 05 (0x05) Write Single Coil
				ex = Handler::RegCoilsWrite( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_WRITE_VALUE_OFF], regAddr, 1 );
				break;
			case ModbusConsts::MB_FUNC_WRITE_REGISTER:				// 06 (0x06) Write Single Register
				ex = Handler::RegHoldingWrite( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_WRITE_VALUE_OFF], regAddr, 1 );
				break;
			case ModbusConsts::MB_FUNC_WRITE_MULTIPLE_COILS:		// 15 (0x0F) Write Multiple Coils
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_WRITE_MUL_COIL_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegCoilsWrite( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], regAddr, regCnt );
				}
				break;
			case ModbusConsts::MB_FUNC_WRITE_MULTIPLE_REGISTERS:	// 16 (0x10) Write Multiple registers
				if( regCnt < 1 || ModbusConsts::MB_PDU_FUNC_WRITE_MUL_REG_CNT_MAX < regCnt ){
					ex = ModbusConsts::MB_EX_ILLEGAL_DATA_VALUE;
				}else{
					ex = Handler::RegHoldingWrite( &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF], regAddr, regCnt );
				}
				break;
			//case ModbusConsts::MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
			//default:
			//	ex=ModbusConsts::MB_EX_ILLEGAL_FUNCTION;
			//	break;
			}
			
			if(ModbusConsts::MB_ADDRESS_BROADCAST == ModbusBase<Uart>::buf[ModbusConsts::MB_SER_PDU_ADDR_OFF]){
				// No response is returned to broadcast requests
				ModbusBase<Uart>::StateStartRecv();
				return true;
			}
			
			uint16_t crc, len;
			if(ex!=ModbusConsts::MB_EX_NONE){
				ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_OFF] = func | ModbusConsts::MB_FUNC_ERROR;
				ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_DATA_OFF] = ex;
				len = 3;
				crc = ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, len);
				ModbusBase<Uart>::buf[len++] = crc&0xff;
				ModbusBase<Uart>::buf[len++] = crc>>8;
				ModbusBase<Uart>::bufLen = len;
			}else{
				switch(func){
				case ModbusConsts::MB_FUNC_READ_COILS:						// 01 (0x01) Read Coils
				case ModbusConsts::MB_FUNC_READ_DISCRETE_INPUTS:			// 02 (0x02) Read Discrete Inputs
				case ModbusConsts::MB_FUNC_READ_HOLDING_REGISTER:			// 03 (0x03) Read Holding Registers
				case ModbusConsts::MB_FUNC_READ_INPUT_REGISTER:				// 04 (0x04) Read Input Registers
				//case ModbusConsts::MB_FUNC_READWRITE_MULTIPLE_REGISTERS:
					ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_LEN] = anslen;
					len = static_cast<uint16_t>(anslen) + ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA;
					crc = ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, len);
					ModbusBase<Uart>::buf[len++] = crc&0xff;
					ModbusBase<Uart>::buf[len++] = crc>>8;
					ModbusBase<Uart>::bufLen = len;
					break;
				case ModbusConsts::MB_FUNC_WRITE_SINGLE_COIL:				// 05 (0x05) Write Single Coil
				case ModbusConsts::MB_FUNC_WRITE_REGISTER:					// 06 (0x06) Write Single Register
					ModbusBase<Uart>::bufLen = ModbusBase<Uart>::bufPos;
					break;
				case ModbusConsts::MB_FUNC_WRITE_MULTIPLE_COILS:			// 15 (0x0F) Write Multiple Coils
				case ModbusConsts::MB_FUNC_WRITE_MULTIPLE_REGISTERS:		// 16 (0x10) Write Multiple registers
					len = 6;
					crc = ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, len);
					ModbusBase<Uart>::buf[len++] = crc&0xff;
					ModbusBase<Uart>::buf[len++] = crc>>8;
					ModbusBase<Uart>::bufLen = len;
					break;
				}
			}

			ModbusBase<Uart>::StateStartSend();
			return true;
		}
	};


	// This class do not care about slave response timeout.
	// So you should care about it in another place.
	template<class Uart>
	class ModbusMaster : public ModbusBase<Uart>
	{
	public:
		typedef enum {
			ERR_NONE,	// correct data received
			ERR_MODBUS, // modbus error (MBExceptionEnum) received
			ERR_DATA,	// invalid data received
		} ErrTypeEnum;
		
	public:
		static inline void Init(){
			ModbusBase<Uart>::bufPos = 0;
			ModbusBase<Uart>::state = ModbusBase<Uart>::ST_RECV_DONE;
			ModbusBase<Uart>::Init();
		}
		
		static inline void RegInputRead( uint8_t deviceAddr, uint16_t regAddr, uint8_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, ModbusConsts::MB_FUNC_READ_INPUT_REGISTER );
		}

		static inline void RegHoldingRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, ModbusConsts::MB_FUNC_READ_HOLDING_REGISTER );
		}
		
		static inline void RegHoldingWrite( uint8_t deviceAddr, uint16_t regAddr, uint16_t regVal ){
			SomeSingleRW( deviceAddr, regAddr, regVal, ModbusConsts::MB_FUNC_WRITE_REGISTER );
		}
		
		static inline void RegDiscreteRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, ModbusConsts::MB_FUNC_READ_DISCRETE_INPUTS );
		}
		
		static inline void RegCoilsRead( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeSingleRW( deviceAddr, regAddr, regCnt, ModbusConsts::MB_FUNC_READ_COILS );
		}
		
		static inline void RegCoilsWrite( uint8_t deviceAddr, uint16_t regAddr, uint16_t regVal ){
			SomeSingleRW( deviceAddr, regAddr, regVal, ModbusConsts::MB_FUNC_WRITE_SINGLE_COIL );
		}
		
		
		// Use this function to put data into buffer, before RegHoldingWriteMultiple or RegCoilsWriteMultiple call
		static inline uint8_t* RegWriteMultiplePrepare(){
			return &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_WRITE_MUL_VALUES_OFF];
		}

		static inline void RegHoldingWriteMultiple( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeMultipleRW( deviceAddr, regAddr, regCnt, regCnt<<1, ModbusConsts::MB_FUNC_WRITE_MULTIPLE_REGISTERS );
		}

		static inline void RegCoilsWriteMultiple( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt ){
			SomeMultipleRW( deviceAddr, regAddr, regCnt, regCnt>>3, ModbusConsts::MB_FUNC_WRITE_MULTIPLE_COILS );
		}

		// return true if some data received from slave
		static inline bool DataReceived(){
			return ModbusBase<Uart>::ST_RECV_DONE == ModbusBase<Uart>::state;
		}
		
		static ErrTypeEnum CheckErrors(){
			if( 	ModbusBase<Uart>::bufPos < ModbusConsts::MB_SER_PDU_SIZE_MIN 
					|| reqAddr != ModbusBase<Uart>::buf[ModbusConsts::MB_SER_PDU_ADDR_OFF]
					|| reqFunc != ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_OFF]
					|| ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, ModbusBase<Uart>::bufPos) != 0 )
			{
				return ERR_DATA;
			}else if( ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_OFF] >= ModbusConsts::MB_FUNC_ERROR ){
				return ERR_MODBUS;
			}else{
				return ERR_NONE;
			}
		}
		
		static ModbusConsts::MBExceptionEnum GetModbusErr(){
			return static_cast<ModbusConsts::MBExceptionEnum>(ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_DATA_OFF]);
		}

		static uint8_t* ReceivedDataGet(){
			return &ModbusBase<Uart>::buf[ModbusConsts::MB_PDU_FUNC_RESPONSE_DATA];
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
			ModbusBase<Uart>::buf[len++] = deviceAddr;
			ModbusBase<Uart>::buf[len++] = func;
			ModbusBase<Uart>::buf[len++] = regAddr>>8;
			ModbusBase<Uart>::buf[len++] = regAddr&0xff;
			ModbusBase<Uart>::buf[len++] = regCntOrVal>>8;
			ModbusBase<Uart>::buf[len++] = regCntOrVal&0xff;
			uint16_t crc = ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, len);
			ModbusBase<Uart>::buf[len++] = crc&0xff;
			ModbusBase<Uart>::buf[len++] = crc>>8;
			ModbusBase<Uart>::bufLen = len;
			ModbusBase<Uart>::StateStartSend();
		}
		// similar functions batch:
		// 15 (0x0F) Write Multiple Coils
		// 16 (0x10) Write Multiple registers
		static void SomeMultipleRW( uint8_t deviceAddr, uint16_t regAddr, uint16_t regCnt, uint8_t byteCnt , uint8_t func){
			reqAddr = deviceAddr;
			reqFunc = func;
			unsigned len = 0;
			ModbusBase<Uart>::buf[len++] = deviceAddr;
			ModbusBase<Uart>::buf[len++] = func;
			ModbusBase<Uart>::buf[len++] = regAddr>>8;
			ModbusBase<Uart>::buf[len++] = regAddr&0xff;
			ModbusBase<Uart>::buf[len++] = regCnt>>8;
			ModbusBase<Uart>::buf[len++] = regCnt&0xff;
			ModbusBase<Uart>::buf[len++] = byteCnt;
			len += byteCnt;
			uint16_t crc = ComputeCrc<Crc16ModbusTable>(ModbusBase<Uart>::buf, len);
			ModbusBase<Uart>::buf[len++] = crc&0xff;
			ModbusBase<Uart>::buf[len++] = crc>>8;
			ModbusBase<Uart>::bufLen = len;
			ModbusBase<Uart>::StateStartSend();
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

