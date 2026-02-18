
#include <dma.h>
#include "K1921VG015.h"
#include <plic.h>
#include <dispatcher.h>
#include <bit>

using namespace Mcucpp;
// align 1024 is required for DMA hardware
DMA_CtrlStruct_TypeDef dma_control_struct[2] __attribute__((aligned(1024)));

static constexpr size_t DmaChannelsNum = 24;

DmaChannel::TransferCompleteCallback callbacks[DmaChannelsNum];



template <int irq_num>
void DmaIrqHandler()
{
    static_assert(irq_num < DmaChannelsNum / 3);

    for (int channel_num = irq_num * 3; channel_num < irq_num * 3 + 3; channel_num++)
    {
        if ((DMA->IRQSTAT & (1 << channel_num)) == 0)
            continue;

        DmaChannel dma(channel_num);
        dma.ClearTransferCompleteFlag();
        dma.Disable();

        DMA->IRQSTATCLR = 1 << channel_num;

        if (callbacks[channel_num])
            GetCurrentDispatcher().SetTask(callbacks[channel_num]);
    }
}

void DmaInt0()
{
    DmaIrqHandler<0>();
}
void DmaInt1()
{
    DmaIrqHandler<1>();
}
void DmaInt2()
{
    DmaIrqHandler<2>();
}
void DmaInt3()
{
    DmaIrqHandler<3>();
}
void DmaInt4()
{
    DmaIrqHandler<4>();
}
void DmaInt5()
{
    DmaIrqHandler<5>();
}
void DmaInt6()
{
    DmaIrqHandler<6>();
}
void DmaInt7()
{
    DmaIrqHandler<7>();
}



struct DmaInit
{
    DmaInit()
    {
        DMA->BASEPTR = reinterpret_cast<uint32_t>(&dma_control_struct[0]);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA0_VECTNUM, DmaInt0);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA1_VECTNUM, DmaInt1);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA2_VECTNUM, DmaInt2);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA3_VECTNUM, DmaInt3);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA4_VECTNUM, DmaInt4);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA5_VECTNUM, DmaInt5);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA6_VECTNUM, DmaInt6);
        PLIC_SetIrqHandler(Plic_Mach_Target, PLIC_DMA7_VECTNUM, DmaInt7);
        for (size_t i = 0; i < 8; i++)
        {
            PLIC_SetPriority(PLIC_DMA0_VECTNUM + i, 1);
            PLIC_IntEnable(Plic_Mach_Target, PLIC_DMA0_VECTNUM + i);
        }
    }
} dma_init;

enum class DmaControlSet
{
    Primary = 0,
    Secondary = 1,
};

static DMA_Channel_TypeDef &Regs(int number, DmaControlSet set = DmaControlSet::Primary)
{
    return dma_control_struct[(int)set].CH[number];
}

void DmaChannel::Enable()
{
    DMA->CFG_bit.MASTEREN = 1;
    DMA->ENSET = 1 << _number;
}

void DmaChannel::Disable()
{
    DMA->ENCLR = 1 << _number;
}

bool DmaChannel::IsEnabled() const
{
    return DMA->ENSET & (1 << _number) && Regs(_number).CHANNEL_CFG_bit.CYCLE_CTRL != 0;
}

void DmaChannel::ConfigureSource(uint32_t address, DataSize size, AddressIncrement increment)
{
    _src_ptr = address;
    uint32_t tmp = _ctrl_reg & ~(DMA_CHANNEL_CFG_SRC_INC_Msk | DMA_CHANNEL_CFG_SRC_SIZE_Msk);
    tmp |= (uint32_t)size << DMA_CHANNEL_CFG_SRC_SIZE_Pos;
    tmp |= (increment == AddressIncrement::Fixed ? (uint32_t)DMA_CHANNEL_CFG_SRC_INC_None : (uint32_t)size) << DMA_CHANNEL_CFG_SRC_INC_Pos;
    _ctrl_reg = tmp;
}

void DmaChannel::ConfigureDestination(uint32_t address, DataSize size, AddressIncrement increment)
{
    _dst_ptr = address;
    uint32_t tmp = _ctrl_reg & ~(DMA_CHANNEL_CFG_DST_INC_Msk | DMA_CHANNEL_CFG_DST_SIZE_Msk);
    tmp |= (uint32_t)size << DMA_CHANNEL_CFG_DST_SIZE_Pos;
    tmp |= (increment == AddressIncrement::Fixed ? (uint32_t)DMA_CHANNEL_CFG_DST_INC_None : (uint32_t)size) << DMA_CHANNEL_CFG_DST_INC_Pos;
    _ctrl_reg = tmp;
}

bool DmaChannel::SetTransferCount(uint16_t count, uint16_t batch_size)
{
    if (count == 0 || count > 1024)
        return false;
    if (batch_size == 0)
        return false;
    if ((batch_size & (batch_size - 1)) != 0) // batch_size must be power of 2
        return false;

    uint32_t tmp = _ctrl_reg & ~(DMA_CHANNEL_CFG_N_MINUS_1_Msk | DMA_CHANNEL_CFG_R_POWER_Msk);
    tmp |= (count - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos;
    uint32_t r_power = std::countr_zero(batch_size);
    tmp |= r_power << DMA_CHANNEL_CFG_R_POWER_Pos;
    _ctrl_reg = tmp;
    _transfer_count = count;
    return true;
}

bool DmaChannel::IsTransferComplete() const
{
    return DMA->IRQSTAT & (1 << _number);
}

void DmaChannel::ClearTransferCompleteFlag()
{
    DMA->IRQSTATCLR = 1 << _number;
}

void DmaChannel::SetPriority(Priority priority)
{
    if (priority == Priority::High)
        DMA->PRIORITYSET = 1 << _number;
    else
        DMA->PRIORITYCLR = 1 << _number;
}

void DmaChannel::SetTransferMode(TransferMode mode, bool circular)
{
    if (circular)
        DMA->CIRCULARSET = 1 << _number;
    else
        DMA->CIRCULARCLR = 1 << _number;

    _ctrl_reg = (_ctrl_reg & ~DMA_CHANNEL_CFG_CYCLE_CTRL_Msk) | (uint32_t)mode << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos;
}

static uint32_t src_inc(uint32_t reg)
{
    uint32_t field = (reg & DMA_CHANNEL_CFG_SRC_INC_Msk) >> DMA_CHANNEL_CFG_SRC_INC_Pos;
    if (field == DMA_CHANNEL_CFG_SRC_INC_None)
        return 0;
    return 1 << field;
}

static uint32_t dst_inc(uint32_t reg)
{
    uint32_t field = (reg & DMA_CHANNEL_CFG_DST_INC_Msk) >> DMA_CHANNEL_CFG_DST_INC_Pos;
    if (field == DMA_CHANNEL_CFG_DST_INC_None)
        return 0;
    return 1 << field;
}

void DmaChannel::StartTransfer()
{
    uint32_t transfers_count_minus_1 = _transfer_count - 1;

    auto &regs = Regs(_number);
    regs.SRC_DATA_END_PTR = _src_ptr + transfers_count_minus_1 * src_inc(_ctrl_reg);
    regs.DST_DATA_END_PTR = _dst_ptr + transfers_count_minus_1 * dst_inc(_ctrl_reg);
    regs.CHANNEL_CFG = _ctrl_reg;

    Enable();
}

void DmaChannel::SetTransferCompleteCallback(TransferCompleteCallback callback)
{
    callbacks[_number] = callback;
    PLIC_IntEnable(Plic_Mach_Target, PLIC_DMA0_VECTNUM + _number / 3);
}

void DmaChannel::SwTrigger()
{
    DMA->SWREQ = 1 << _number;
}

bool DmaChannel::TransferComplete() const
{
    return (DMA->IRQSTAT & (1u << _number)) == (1u << _number);
}

uint16_t DmaChannel::TransferedCount() const
{
    auto &regs = Regs(_number);
    if ((DMA->IRQSTAT & (1u << _number)) != 0)
        return _transfer_count;
    return _transfer_count - regs.CHANNEL_CFG_bit.N_MINUS_1 - 1;
}
