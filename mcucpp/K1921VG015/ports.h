#pragma once

#include <ioreg.h>

#include <K1921VG015.h>
#include <debug.h>
#include <enum.h>

namespace Mcucpp
{
	namespace IO
	{
		enum class PortCfg
		{
			In = 0,
			Out = 1,
			AltFunc = 2,
		};

		enum class PullMode
		{
			NoPullUp = 0,
			PullUp = 1,
		};

		enum class DriverType
		{
			PushPull = 0,
			OpenDrain = 1,
			OpenSource = 2,
		};

		enum class PortSpeed
		{
			Slow = 0,
			Medium = 1,
			Fast = 2,
			Fastest = 3
		};

		enum class InterruptCfg
		{
			Disabled = 0,
			Enabled = 1,
			Level = 0,
			Front = 2,
			NegPol = 0,
			PosPol = 4,
			AnyPol = 8,
		};

		inline constexpr uint32_t ExpandMask2bits(uint32_t mask)
		{
			mask = (mask & 0xff00) << 8 | (mask & 0x00ff);
			mask = (mask & 0x00f000f0) << 4 | (mask & 0x000f000f);
			mask = (mask & 0x0C0C0C0C) << 2 | (mask & 0x03030303);
			mask = (mask & 0x22222222) << 1 | (mask & 0x11111111);
			return mask;
		}

		inline constexpr uint32_t ExpandMask4bits(uint32_t mask)
		{
			mask = (mask & 0xf0) << 12 | (mask & 0x0f);
			mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
			mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
			return mask;
		}

		inline constexpr uint32_t UnpackValue2bits(uint32_t mask, uint32_t value, uint32_t configuration)
		{
			return (value & ~(mask * 0x03)) | mask * configuration;
		}

		inline constexpr uint32_t UnpackValue4Bit(uint32_t mask, uint32_t value, uint32_t configuration)
		{
			return (value & ~(mask * 0x0f)) | mask * configuration;
		}

		DECLARE_ENUM_OPERATIONS(InterruptCfg);

		constexpr size_t NumGpio = 3;

		class GpioPort
		{
			inline GPIO_TypeDef *Regs() { return reinterpret_cast<GPIO_TypeDef *>(GPIOA_BASE + (GPIOB_BASE - GPIOA_BASE) * _id); }
			uint8_t _id;

		public:
			using DataT = uint16_t;
			int Id() { return _id; }

			static constexpr int Width = 16;

			explicit GpioPort(uint8_t id) : _id{id} { MCUCPP_ASSERT(id < NumGpio); }

			DataT Read()
			{
				return Regs()->DATAOUT;
			}
			void Write(DataT value)
			{
				Regs()->DATAOUT = value;
			}

			void WriteMasked(DataT mask, DataT value)
			{
				Regs()->DATAOUTCLR = mask;
				Regs()->DATAOUTSET = value;

				// Regs()->MASKLB[mask & 0xff].MASKLB = value;
				// Regs()->MASKHB[mask >> 8].MASKHB = value >> 8;
			}

			void Set(DataT value)
			{
				Regs()->DATAOUTSET = value;
			}
			void Clear(DataT value)
			{
				Regs()->DATAOUTCLR = value;
			}

			void Toggle(DataT value)
			{
				Regs()->DATAOUTTGL = value;
			}
			DataT PinRead()
			{
				return Regs()->DATA;
			}

			void SetSpeed(DataT, PortSpeed)
			{
			}

			void SetPullUp(DataT mask, PullMode pull)
			{
				Regs()->PULLMODE = (Regs()->PULLMODE & ~mask) | mask * (uint32_t)pull;
			}

			void SetDriverType(DataT mask, DriverType driver)
			{
				Regs()->OUTMODE = UnpackValue2bits(ExpandMask2bits(mask), Regs()->OUTMODE, (uint32_t)driver);
			}

			void AltFuncNumber(DataT mask, uint8_t number)
			{
				Regs()->ALTFUNCNUM = UnpackValue2bits(ExpandMask2bits(mask), Regs()->ALTFUNCNUM, number);
			}

			void SetConfiguration(DataT mask, PortCfg configuration)
			{
				switch (configuration)
				{
				case PortCfg::In:
					Regs()->ALTFUNCCLR = mask;
					Regs()->OUTENCLR = mask;
					break;
				case PortCfg::Out:
					Regs()->ALTFUNCCLR = mask;
					Regs()->OUTENSET = mask;
					break;
				case PortCfg::AltFunc:
					Regs()->OUTENSET = mask;
					Regs()->ALTFUNCSET = mask;
					break;
				}
			}

			void Enable()
			{
				RCU->CGCFGAHB |= (1u << (RCU_CGCFGAHB_GPIOAEN_Pos + _id));
				RCU->RSTDISAHB |= (1u << (RCU_RSTDISAHB_GPIOAEN_Pos + _id));
			}

			void Disable()
			{
				RCU->CGCFGAHB &= ~(1u << (RCU_CGCFGAHB_GPIOAEN_Pos + _id));
			}

			void SetInterruptConfiguration(DataT mask, InterruptCfg configuration)
			{
				if (HasAnyFlag(configuration, InterruptCfg::Enabled))
				{
					Regs()->INTENCLR = mask;
					if (HasAnyFlag(configuration, InterruptCfg::Front))
						Regs()->INTTYPESET = mask;
					else
						Regs()->INTTYPECLR = mask;

					if (HasAnyFlag(configuration, InterruptCfg::PosPol))
						Regs()->INTPOLSET = mask;
					else
						Regs()->INTPOLCLR = mask;

					if (HasAnyFlag(configuration, InterruptCfg::AnyPol))
						Regs()->INTPOLSET = mask;
					else
						Regs()->INTPOLCLR = mask;

					Regs()->INTENSET = mask;
				}
				else
				{
					Regs()->INTENCLR = mask;
				}
			}
		};

		struct NativePortBase
		{
			using Configuration = PortCfg;
			using Speed = PortSpeed;
			using DriverType = DriverType;
			using PullMode = PullMode;

			using DataT = uint16_t;
			static constexpr int Width = 16;

			static constexpr auto AltFunc = PortCfg::AltFunc;
			static constexpr auto In = PortCfg::In;
			static constexpr auto Out = PortCfg::Out;
		};

		template <int ID>
		class StaticGpioPort : public NativePortBase
		{

		public:
			static constexpr uint8_t Id = ID;

			static DataT Read() { return GpioPort(ID).Read(); }
			static void Write(DataT value) { GpioPort(ID).Write(value); }
			static void ClearAndSet(DataT mask, DataT value) { GpioPort(ID).WriteMasked(mask, value); }
			static void Set(DataT value) { GpioPort(ID).Set(value); }
			static void Clear(DataT value) { GpioPort(ID).Clear(value); }
			static void Toggle(DataT value) { GpioPort(ID).Toggle(value); }
			static DataT PinRead() { return GpioPort(ID).PinRead(); }
			static void SetSpeed(DataT mask, PortSpeed speed) { GpioPort(ID).SetSpeed(mask, speed); }
			static void SetPullUp(DataT mask, PullMode pull) { GpioPort(ID).SetPullUp(mask, pull); }
			static void SetDriverType(DataT mask, DriverType driver) { GpioPort(ID).SetDriverType(mask, driver); }
			static void AltFuncNumber(DataT mask, uint8_t number) { GpioPort(ID).AltFuncNumber(mask, number); }
			static void SetConfiguration(DataT mask, PortCfg configuration) { GpioPort(ID).SetConfiguration(mask, configuration); }
			static void Enable() { GpioPort(ID).Enable(); }
			static void Disable() { GpioPort(ID).Disable(); }
		};

		using Porta = StaticGpioPort<0>;
		using Portb = StaticGpioPort<1>;
		using Portc = StaticGpioPort<2>;

#define MCUCPP_HAS_PORTA 1
#define MCUCPP_HAS_PORTB 1
#define MCUCPP_HAS_PORTC 1

	}
}
