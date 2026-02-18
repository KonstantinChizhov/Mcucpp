
#include <adc.h>
#include <dma.h>
#include <clock.h>
#include <K1921VG015.h>
#include <numeric>

using namespace Mcucpp;

static AdcTrigger _trigger = AdcTrigger::SwReq;
static AdcCallbackType adc_callback;
static DmaChannel dma(DMA_CH_ADCSARSEQ0);

void Adc1::SetSequenceCallback(AdcCallbackType callback)
{
    adc_callback = callback;
}

void Adc1::SetTrigger(AdcTrigger trigger)
{
    _trigger = trigger;
}

void Adc1::Init(int divider, AcdClockSource clock_crc)
{
    PMUSYS->ADCPWRCFG_bit.LDOEN = 1;
    PMUSYS->ADCPWRCFG_bit.LVLDIS = 0;

    RCU->ADCSARCLKCFG_bit.RSTDIS = 0; // reset ADC
    RCU->RSTDISAPB_bit.ADCSAREN = 0;

    uint32_t cfg_reg = RCU_ADCSARCLKCFG_CLKEN_Msk | RCU_ADCSARCLKCFG_RSTDIS_Msk | (uint32_t)clock_crc;
    if (divider > 1)
    {
        cfg_reg |= RCU_ADCSARCLKCFG_DIVEN_Msk;
        cfg_reg |= (divider / 2 - 1) << RCU_ADCSARCLKCFG_DIVN_Pos;
    }

    RCU->ADCSARCLKCFG = cfg_reg;

    RCU->CGCFGAPB_bit.ADCSAREN = 1;
    RCU->RSTDISAPB_bit.ADCSAREN = 1;

    ADCSAR->ACTL = ADCSAR_ACTL_ADCEN_Msk | ADCSAR_ACTL_CALEN_Msk | (3 << ADCSAR_ACTL_SELRES_Pos);

    // Ждем пока АЦП пройдут инициализацию, начатую в самом начале
    while (!(ADCSAR->ACTL_bit.ADCRDY))
    {
    }

    SetSampleTime(Adc1::TempSensorChannel, 300);
}

void ClearAdc()
{
    ADCSAR->EMUX_bit.EM0 = 0;
    ADCSAR->SEQ[0].SDMACTL = 0;
    ADCSAR->SEQEN_bit.SEQEN0 = 0;
    ADCSAR->SEQSYNC_bit.SYNC0 = 0;
    ADCSAR->FSTAT = ADCSAR_FSTAT_OV0_Msk | ADCSAR_FSTAT_UN0_Msk | ADCSAR_FSTAT_DOV0_Msk;

    dma.ClearTransferCompleteFlag();
    dma.Disable();

    // clear the FIFO
    while (ADCSAR->SEQ[0].SFLOAD_bit.VAL)
    {
        (void)ADCSAR->SEQ[0].SFIFO;
    }
    ADCSAR->IC = ADCSAR_IC_SEQIC0_Msk;
    _trigger = AdcTrigger::SwReq;
}

void DmaCallback()
{
    ClearAdc();
    if (adc_callback)
        adc_callback();
}

bool Adc1::StartSequence(std::span<const uint8_t> channels, std::span<DataT> sample_buffer, uint32_t delay_cicles)
{
    if (channels.size() == 0 || sample_buffer.size() == 0)
        return false;

    if (channels.size() > Adc1::ChannelsCount)
        return false;

    ClearAdc();

    auto &seq = ADCSAR->SEQ[0];

    // Настройка секвенсора 0: CH0 - CH7
    ADCSAR->EMUX_bit.EM0 = (uint32_t)_trigger;

    seq.SCCTL = 0;
    seq.SRTMR = delay_cicles;

    seq.SRQCTL = ((channels.size() - 1) << ADCSAR_SEQ_SRQCTL_RQMAX_Pos) | (0 << ADCSAR_SEQ_SRQCTL_QAVGVAL_Pos) | (0 << ADCSAR_SEQ_SRQCTL_QAVGEN_Pos);

    uint32_t srqsel = 0;
    for (size_t ch = 0; ch < channels.size(); ch++)
    {
        srqsel |= (channels[ch] & 0x0f) << (4 * ch);
    }

    seq.SRQSEL = srqsel;

    // Включаем секвенсоры
    ADCSAR->SEQEN = ADCSAR_SEQEN_SEQEN0_Msk;

    ADCSAR->FSTAT = ADCSAR_FSTAT_OV0_Msk | ADCSAR_FSTAT_UN0_Msk | ADCSAR_FSTAT_DOV0_Msk;
    ADCSAR->IC = ADCSAR_IC_SEQIC0_Msk;

    seq.SDMACTL = ADCSAR_SEQ_SDMACTL_DMAEN_Msk | (ADCSAR_SEQ_SDMACTL_WMARK_Level16 << ADCSAR_SEQ_SDMACTL_WMARK_Pos);

    dma.ConfigureSource((uint32_t)&ADCSAR->SEQ[0].SFIFO, DmaChannel::DataSize::HalfWord, DmaChannel::AddressIncrement::Fixed);
    dma.ConfigureDestination((uint32_t)sample_buffer.data(), DmaChannel::DataSize::HalfWord, DmaChannel::AddressIncrement::Increment);
    dma.SetTransferCount(sample_buffer.size(), 16);
    dma.SetTransferMode(DmaChannel::TransferMode::Normal, false);

    dma.SetTransferCompleteCallback(DmaCallback);
    dma.StartTransfer();

    // trigger ADC
    ADCSAR->SEQSYNC_bit.SYNC0 = 1;
    ADCSAR->SEQSYNC_bit.GSYNC = 1;

    return true;
}

Adc1::DataT Adc1::ReadImmediate(uint8_t channel, unsigned avg_count, uint32_t delay_cicles)
{
    while (!Ready())
        ;

    // Настройка секвенсора : CH0 - CH7
    ADCSAR->EMUX_bit.EM0 = (uint32_t)_trigger;
    auto &seq = ADCSAR->SEQ[0];

    uint32_t avgr_count = avg_count > 0 ? (1 << std::min(8, std::countr_zero(avg_count))) - 1 : 0;
    uint32_t avg_enable = avg_count > 0 ? 1 : 0;
    seq.SCCTL = (avgr_count << ADCSAR_SEQ_SCCTL_RCNT_Pos) | (avg_enable << ADCSAR_SEQ_SCCTL_RAVGEN_Pos) | (0 << ADCSAR_SEQ_SCCTL_ICNT_Pos);
    seq.SRTMR = delay_cicles;
    seq.SRQCTL = (0 << ADCSAR_SEQ_SRQCTL_RQMAX_Pos) | (0 << ADCSAR_SEQ_SRQCTL_QAVGVAL_Pos) | (0 << ADCSAR_SEQ_SRQCTL_QAVGEN_Pos);
    seq.SRQSEL_bit.RQ0 = channel & 0x0f;
    seq.SDMACTL = 0;

    // Включаем секвенсоры
    ADCSAR->SEQEN_bit.SEQEN0 = 1;
    ADCSAR->FSTAT = ADCSAR_FSTAT_OV0_Msk | ADCSAR_FSTAT_UN0_Msk | ADCSAR_FSTAT_DOV0_Msk;
    ADCSAR->IC = ADCSAR_IC_SEQIC0_Msk;

    ADCSAR->SEQSYNC_bit.SYNC0 = 1;
    ADCSAR->SEQSYNC_bit.GSYNC = 1;

    while ((ADCSAR->BSTAT))
        ;
    while (!(ADCSAR->RIS_bit.SEQRIS0))
        ; // Ожидание флага прерывания секвенсора 0

    DataT result = seq.SFIFO;

    ADCSAR->EMUX_bit.EM0 = 0;
    ADCSAR->SEQEN_bit.SEQEN0 = 0;
    ADCSAR->SEQSYNC_bit.SYNC0 = 0;
    ADCSAR->FSTAT = ADCSAR_FSTAT_OV0_Msk | ADCSAR_FSTAT_UN0_Msk | ADCSAR_FSTAT_DOV0_Msk;

    // clear the FIFO
    while (seq.SFLOAD_bit.VAL)
    {
        (void)seq.SFIFO;
    }
    ADCSAR->IC = ADCSAR_IC_SEQIC0_Msk;

    return result;
}

void Adc1::Disable()
{
    PMUSYS->ADCPWRCFG_bit.LDOEN = 0;
    RCU->RSTDISAPB_bit.ADCSAREN = 0;
}

void Adc1::SetSampleTime(uint8_t channel, uint16_t adcCycles)
{
    if (channel >= std::size(ADCSAR->CHDELAY))
        return;
    ADCSAR->CHDELAY[channel].CHDELAY = adcCycles;
}

void Adc1::SetResolution(AdcResolution bits)
{
    ADCSAR->ACTL = ADCSAR_ACTL_ADCEN_Msk | ((uint32_t)bits << ADCSAR_ACTL_SELRES_Pos);
}

uint32_t Adc1::ClockFreq()
{
    uint32_t clk = 0;
    switch (RCU->ADCSARCLKCFG_bit.CLKSEL)
    {
    case RCU_ADCSARCLKCFG_CLKSEL_HSI:
        clk = Clock::HsiClock::ClockFreq();
        break;
    case RCU_ADCSARCLKCFG_CLKSEL_HSE:
        clk = Clock::HseClock::ClockFreq();
        break;
    case RCU_ADCSARCLKCFG_CLKSEL_PLL0:
        clk = Clock::PllClock::ClockFreq();
        break;
    case RCU_ADCSARCLKCFG_CLKSEL_PLL1: // TODO: implement PLL1
        clk = Clock::PllClock::ClockFreq();
        break;
    }
    if (!RCU->ADCSARCLKCFG_bit.DIVEN)
        return clk;
    uint32_t div = (RCU->ADCSARCLKCFG_bit.DIVN + 1) * 2;
    return clk / div;
}

int Adc1::ResolutionBits()
{
    return ADCSAR->ACTL_bit.SELRES * 2 + 6;
}

void Adc1::EnableTSensor()
{
    TSENS->CTRL = TSENS_CTRL_ADCSEL_Msk | TSENS_CTRL_ISEL_Msk;
}

float Adc1::ToVolts(uint16_t value)
{
    uint32_t max_value = (1 << ResolutionBits());
    return AREF * value / max_value;
}

uint32_t Adc1::ConvertionTimeCycles(uint8_t channel)
{
    return ResolutionBits() + 2 + ADCSAR->CHDELAY[channel].CHDELAY_bit.DELAY;
}

unsigned Adc1::AdcPeriod(uint8_t channel)
{
    unsigned adcTickNs4 = (4'000'000'000u / ClockFreq());
    unsigned adcTickNs10 = adcTickNs4 * 2 + adcTickNs4 / 2;
    return (adcTickNs10 * ConvertionTimeCycles(channel) + adcTickNs10 / 2) / 10;
}

float Adc1::ConvertTemperature(uint16_t raw_adc_value)
{
    uint32_t max_value = (1 << ResolutionBits());
    float tf = 296.9f - 192.7667f * raw_adc_value * AREF / max_value;
    return tf;
}

bool Adc1::Ready()
{
    return (ADCSAR->BSTAT & (ADCSAR_BSTAT_ADCBUSY_Pos | ADCSAR_BSTAT_SEQBUSY0_Pos | ADCSAR_BSTAT_SEQBUSY1_Pos)) == 0;
}

void Adc1::SwTrigger()
{
    ADCSAR->SEQSYNC_bit.GSYNC = 1;
}