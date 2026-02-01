//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2025
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

#include <stdint.h>
#include <stddef.h>
#include <span>
#include <clock.h>
#include <ports.h>
#include <iopins.h>
#include <pinlist.h>
#include <enum.h>
#include <dma.h>
#include <plic.h>
#include <span>
#include <data_transfer.h>
#include <optional>
#include <sys_time.h>

namespace Mcucpp
{

    using Uart0RxPins = IO::PinList<IO::Pa0, IO::Pb6>;
    using Uart1RxPins = IO::PinList<IO::Pa2, IO::Pa10>;
    using Uart2RxPins = IO::PinList<IO::Pa4, IO::Pa12>;
    using Uart3RxPins = IO::PinList<IO::Pa6, IO::Pa14>;
    using Uart4RxPins = IO::PinList<IO::Pa8, IO::Pb0>;

    using Uart0TxPins = IO::PinList<IO::Pa1, IO::Pb7>;
    using Uart1TxPins = IO::PinList<IO::Pa3, IO::Pa11>;
    using Uart2TxPins = IO::PinList<IO::Pa5, IO::Pa13>;
    using Uart3TxPins = IO::PinList<IO::Pa7, IO::Pa15>;
    using Uart4TxPins = IO::PinList<IO::Pa9, IO::Pb1>;

    enum class UartError : uint32_t
    {
        NoError = 0,
        OverrunError = UART_RSR_OE_Msk,
        FramingError = UART_RSR_FE_Msk,
        ParityError = UART_RSR_PE_Msk
    };

    enum class UartClockSrc : uint32_t
    {
        Pll0 = RCU_UARTCLKCFG_CLKSEL_PLL0,
        Pll1 = RCU_UARTCLKCFG_CLKSEL_PLL1,
        Hsi = RCU_UARTCLKCFG_CLKSEL_HSI,
        Hse = RCU_UARTCLKCFG_CLKSEL_HSE
    };

    constexpr int UartNum = 5;
    constexpr int AltPinsCount = 2;
    template <int Number, class TxPins, class RxPins>
    class Uart
    {
        static inline sys_clock::duration _rx_char_timeout = 0;

    public:
        static constexpr uint32_t ErrorMask = UART_RSR_OE_Msk | UART_RSR_FE_Msk | UART_RSR_PE_Msk;

        static UartError GetError()
        {
            return static_cast<UartError>(Regs()->RSR & ErrorMask);
        }

        static inline UART_TypeDef *Regs() { return reinterpret_cast<UART_TypeDef *>(UART0_BASE + (UART1_BASE - UART0_BASE) * Number); }

        static_assert(Number < UartNum);

        static bool HasDma() noexcept { return Number < 3; }

        static void Init(uint32_t baud, UartClockSrc clock_source = UartClockSrc::Pll0)
        {
            RCU->RSTDISAPB &= ~(1 << (RCU_RSTDISAPB_UART0EN_Pos + Number));
            RCU->CGCFGAPB |= 1 << (RCU_CGCFGAPB_UART0EN_Pos + Number);
            RCU->RSTDISAPB |= 1 << (RCU_RSTDISAPB_UART0EN_Pos + Number);

            RCU->UARTCLKCFG[Number].UARTCLKCFG_bit.CLKSEL = static_cast<uint32_t>(clock_source);
            RCU->UARTCLKCFG[Number].UARTCLKCFG_bit.DIVEN = 0;
            RCU->UARTCLKCFG[Number].UARTCLKCFG_bit.RSTDIS = 1;
            RCU->UARTCLKCFG[Number].UARTCLKCFG_bit.CLKEN = 1;
            
            uint32_t src_clock = Clock::SysClock::ClockFreq();
            if(clock_source == UartClockSrc::Hsi)
                src_clock = Clock::HsiClock::ClockFreq();
            if(clock_source == UartClockSrc::Hse)
                src_clock = Clock::HseClock::ClockFreq();
            
                
            uint32_t baud_icoef = src_clock / (16 * baud);
            uint32_t baud_fcoef = ((src_clock / (16.0f * baud) - baud_icoef) * 64 + 0.5f);

            Regs()->IBRD = baud_icoef;
            Regs()->FBRD = baud_fcoef;

            Regs()->LCRH = (3 << UART_LCRH_WLEN_Pos) | UART_LCRH_STP2_Msk;

            // FIFO does not work correctly with DMA. So use it only for UARTs 3 and 4 with no DMA
            if (!HasDma())
                Regs()->LCRH |= UART_LCRH_FEN_Msk;

            Regs()->IFLS = 0;
            Regs()->CR = UART_CR_TXE_Msk | UART_CR_RXE_Msk | UART_CR_UARTEN_Msk;

            PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_UART0_VECTNUM, UartInterrupt);
        }

        static void UartInterrupt()
        {

        }

        static void Disable()
        {
            RCU->RSTDISAPB &= ~(1 << (RCU_RSTDISAPB_UART0EN_Pos + Number));
        }

        static void SelectTxRxPins(int rx, int tx, IO::DriverType driver_type = IO::DriverType::PushPull, IO::PullMode pull_mode = IO::PullMode::NoPullUp)
        {
            typedef typename TxPins::ValueType Type;
            TxPins::Enable();
            Type maskTx(1 << tx);
            TxPins::SetConfiguration(maskTx, TxPins::AltFunc);
            TxPins::AltFuncNumber(maskTx, tx == 0 ? 1 : 3);
            TxPins::SetDriverType(maskTx, driver_type);
            TxPins::SetPullUp(maskTx, pull_mode);

            RxPins::Enable();
            Type maskRx(1 << rx);
            RxPins::SetConfiguration(maskRx, RxPins::AltFunc);
            RxPins::AltFuncNumber(maskRx, rx == 0 ? 1 : 3);
            RxPins::SetPullUp(maskRx, pull_mode);
        }

        template <class TxPin, class RxPin>
        static void SelectTxRxPins()
        {
            const int txPinIndex = TxPins::template PinIndex<TxPin>::Value;
            const int rxPinIndex = RxPins::template PinIndex<RxPin>::Value;
            static_assert(txPinIndex >= 0);
            static_assert(rxPinIndex >= 0);
            SelectTxRxPins(txPinIndex, rxPinIndex);
        }

        static void SetRxTimeout(sys_clock::duration rxTimeoutChars)
        {
            _rx_char_timeout = rxTimeoutChars;
        }

        static void Write(uint8_t data)
        {
            while ((Regs()->FR & UART_FR_BUSY_Msk) != 0)
                ;
            Regs()->DR = data;
        }

        static void Write(std::span<const uint8_t> data)
        {
            for (auto ch : data)
            {
                while ((Regs()->FR & UART_FR_BUSY_Msk) != 0)
                    ;
                Regs()->DR = ch;
            }
        }

        static bool Write(const void *data, size_t size)
        {
            Write(std::span<uint8_t>{(uint8_t *)data, size});
            return true;
        }

        static bool WriteAsync(const void *data, size_t size, TransferCallbackFunc callback)
        {
            if (size == 0)
                return false;
            if (!HasDma())
            {
                Write(data, size);
                callback((void*)data, size, true);
            }

            DmaChannel dma(DMA_CH_UART0TX + Number);
            // wait TX complete
            while ((Regs()->FR & UART_FR_BUSY_Msk) != 0)
                ;

            Regs()->ICR = UART_ICR_TDIC_Msk;

            dma.ClearTransferCompleteFlag();
            Regs()->DMACR_bit.TXDMAE = 0;

            dma.ConfigureDestination((uint32_t)&Regs()->DR, DmaChannel::DataSize::Byte, DmaChannel::AddressIncrement::Fixed);
            dma.ConfigureSource((uint32_t)data, DmaChannel::DataSize::Byte, DmaChannel::AddressIncrement::Increment);
            dma.SetTransferCount(size, 1);
            dma.SetTransferMode(DmaChannel::TransferMode::Normal, false);
            dma.StartTransfer();
            dma.SetTransferCompleteCallback(callback);

            Regs()->DMACR_bit.TXDMAE = 1;
            
            //co_await dma.TransferComplete();

            Regs()->DMACR_bit.TXDMAE = 0;
            Regs()->ICR = UART_ICR_TDIC_Msk;

            dma.ClearTransferCompleteFlag();
            dma.Disable();
            // co_await io_flag<uint32_t>(&Regs()->FR, UART_FR_BUSY_Msk, 0);
        }

        bool RxAvailable()
        {
            return (Regs()->FR & UART_FR_RXFE_Msk) == 0;
        }

        std::optional<uint8_t> Read()
        {
            bool timeout = false;
            auto start_time = sys_clock::now();

            while (!RxAvailable())
            {
                if (sys_clock::now() - start_time >= _rx_char_timeout)
                {
                    timeout = true;
                    break;
                }
            }

            if (timeout)
            {
                return std::optional<uint8_t>{};
            }
            return (uint8_t) Regs()->DR;
        }

        static bool ReadAsync(void *data, size_t size, TransferCallbackFunc callback)
        {
            if (!HasDma())
            {
                size_t count = 0;
                for (uint8_t &byte : data)
                {
                    std::optional<uint8_t> ch = Read();
                    if (!ch.has_value()) // timeout has occured
                        break;
                    byte = ch.value();
                    count++;
                }
                return count;
            }

            DmaChannel dma(DMA_CH_UART0RX + Number);

            if (dma.IsEnabled())
                dma.ClearTransferCompleteFlag();
            Regs()->DMACR_bit.RXDMAE = 0;

            dma.ConfigureSource((uint32_t)&Regs()->DR, DmaChannel::DataSize::Byte, DmaChannel::AddressIncrement::Fixed);
            dma.ConfigureDestination((uint32_t)data, DmaChannel::DataSize::Byte, DmaChannel::AddressIncrement::Increment);
            dma.SetTransferCount(size, 1);
            dma.SetTransferMode(DmaChannel::TransferMode::Normal, false);
            dma.StartTransfer();
            Regs()->DMACR_bit.RXDMAE = 1;

            uint16_t transfer_count = 0, last_transfer_count = 0;
            bool timeout;
            do
            {
                last_transfer_count = transfer_count;
                //timeout = dma.TransferComplete().with_timeout(_rx_char_timeout);
                transfer_count = dma.TransferedCount();

            } while (transfer_count != last_transfer_count);
            if (!timeout)
                dma.ClearTransferCompleteFlag();
            Regs()->DMACR_bit.RXDMAE = 0;

            return transfer_count;
        }
    };

    DECLARE_ENUM_OPERATIONS(UartError);

    using Usart0 = Uart<0, Uart0TxPins, Uart0RxPins>;
    using Usart1 = Uart<1, Uart1TxPins, Uart1RxPins>;
    using Usart2 = Uart<2, Uart2TxPins, Uart2RxPins>;
    using Usart3 = Uart<3, Uart3TxPins, Uart3RxPins>;
    using Usart4 = Uart<4, Uart4TxPins, Uart4RxPins>;

}
