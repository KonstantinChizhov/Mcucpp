
#ifndef __KS0108__
#define __KS0108__

#include <static_assert.h>
#include <delay.h>
#include <iopins.h>
#include <template_utils.h>

namespace Drivers
{
    using namespace Clock;
	using namespace Util;

    class KS0108Base
    {
    public:
        enum ControlBusBits
        {
            Cs1Bit = 1,
            Cs2Bit = 2,
            RwBit = 8,
            DiBit = 16
                };

        enum Commands
        {
            DisplayOnCmd = 0x3F,
            DisplayOffCmd = 0x3E,
            SetAddressCmd = 0x40,
            SetPageCmd = 0xB8,
            StartLineCmd = 0xC0
                       };

        enum OutputMode
        {
            WriteMode,
            XorMode,
            AndMode
        };

        typedef int_fast16_t Coord;
        typedef uint_fast8_t Color;
		static const Color DefaultColor = 1;

        static Coord Width() {return 128;}
        static Coord Height() {return 64;}
    protected:

    };


    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            class KS0108 :public KS0108Base
    {
        BOOST_STATIC_ASSERT(DataBus::Length == 8);
        typedef IO::PinList<Cs1, Cs2, Reset, Rw, Di, E> ControlBus;
        typedef IO::PinList<Cs1, Cs2, IO::NullPin, Rw, Di> Controls;
        using KS0108Base::OutputMode;
        using KS0108Base::Commands;
        using KS0108Base::ControlBusBits;
        static const unsigned PulseDelay = 7; // microsec
    public:
	 	using KS0108Base::Coord;
        using KS0108Base::Color;
        static void Init();
        static void Fill(Color color);
        static void PutPixel(Coord x, Coord y, Color color);
        template<class BitmapT>
        static void DrawBitmap(BitmapT &bitmap, Coord x, Coord y);
        static void Flush();
        inline static void SetOutputMode(OutputMode mode)
        {
            _mode = mode;
        }
    protected:
        static void Write(uint8_t data);
        static void SetPage(uint_fast8_t page, Coord x);
        static void WritePage(uint8_t data, uint_fast8_t pageStart, uint_fast8_t pageEnd, Coord x);
        static void PulseE();
        static OutputMode _mode;
    };

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            KS0108Base::OutputMode KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::_mode = KS0108Base::WriteMode;

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::PulseE()
    {
        E::Set();
        Util::delay_us<PulseDelay, SysClock::CpuFreq>();
        E::Clear();
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::PutPixel(Coord x, Coord y, Color color)
    {
        if(x < 0 || x > 127) return;
        if(x  > 63)
        {
            Cs2::Set();
            Cs1::Clear();
            x -= 64;
        }
        else
        {
            Cs2::Clear();
            Cs1::Set();
        }

        Coord page = y >> 3;
        Di::Clear();

        Write(SetPageCmd | page);
        Write(SetAddressCmd | x);

        Di::Set(); Rw::Set();
        DataBus::template SetConfiguration<0xff, DataBus::In>();
        PulseE();
        Color mask = 1 << (y & 0x07);
        E::Set();
        Util::delay_us<PulseDelay, SysClock::CpuFreq>();
        Color data = DataBus::PinRead();
        E::Clear();
        Rw::Clear();
        DataBus::template SetConfiguration<0xff, DataBus::Out>();
        Di::Clear();

        if(_mode == XorMode)
        {
            data ^= mask;
        }
        else
            if(color)
            {
            if(_mode == WriteMode)
                data |= mask;
        }
        else
            data &= ~mask;

        Write(SetAddressCmd | x);
        Di::Set();
        Write(data);
        Di::Clear();
    }


    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Init()
    {
        ControlBus::template SetConfiguration<0xff, ControlBus::Out>();
        DataBus::template SetConfiguration<0xff, DataBus::Out>();
        Reset::Clear();
        Util::delay_ms<10, SysClock::CpuFreq>();
        Reset::Set();
        Util::delay_ms<10, SysClock::CpuFreq>();
        Controls::template Write<Cs1Bit | Cs2Bit>();
        Write(DisplayOnCmd);
        Write(SetAddressCmd);
        Write(SetPageCmd);
        Write(StartLineCmd);
        Fill(0);
        Flush();
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Fill(Color color)
    {
        if(color) color = 0xff;
        for(uint8_t i = 0; i < 8; i++)
        {
            Controls::template Write<Cs1Bit | Cs2Bit>();
            Write(SetPageCmd + i);
            Write(SetAddressCmd + 0);
            Controls::template Write<Cs1Bit | Cs2Bit | DiBit>();
            DataBus::Write(color);
            for(uint8_t j = 0; j < 64; j++)
            {
                PulseE();
            }
        }
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Write(uint8_t cmd)
    {
        DataBus::Write(cmd);
        PulseE();
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::SetPage(uint_fast8_t page, Coord x)
    {
        if(x < 0)
            x = 0;
        Controls::template Write<Cs1Bit | Cs2Bit>();
        Write(SetPageCmd | page);
        uint_fast8_t cs2addr = 0;
        if(x > 63)
            cs2addr = x - 64;
        else
        {
            Cs2::Clear();
            Write(SetAddressCmd + x);
        }
        Cs1::Clear();
        Cs2::Set();
        Write(SetAddressCmd + cs2addr);
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::WritePage(uint8_t data, uint_fast8_t pageStart, uint_fast8_t pageEnd, Coord cx)
    {
        if(cx < 0) return;
        if(cx  > 63)
        {
            Cs2::Set();
            Cs1::Clear();
            cx -= 64;
        }
        else
        {
            Cs2::Clear();
            Cs1::Set();
        }

        Di::Set();
        if((pageEnd | pageStart) & 7 || _mode != WriteMode)
        {
            DataBus::template SetConfiguration<0xff, DataBus::In>();
            Rw::Set();
            PulseE();

            uint8_t mask = (1 << (pageStart & 7)) - 1;
            mask |= 0xFE << ((pageEnd - 1) & 7);
            data >>= 7 - ((pageEnd - 1) & 7);

            E::Set();
            Util::delay_us<PulseDelay, SysClock::CpuFreq>();
            uint8_t dest = DataBus::PinRead();
            if(_mode == XorMode)
            {
                data ^= dest;
            }
            else
                if(_mode == AndMode)
                    data &= dest;
            else
                data = (dest & mask) | data;

            E::Clear();
            DataBus::template SetConfiguration<0xff, DataBus::Out>();

            Rw::Clear();
            Di::Clear();
            Write(SetAddressCmd + cx);
            Di::Set();
        }
        Write(data);
    }


    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            template<class BitmapT>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::DrawBitmap(BitmapT &bitmap, Coord x, Coord y)
    {
        if(y > 63 || x > 127) return;
        if(x < -(int)bitmap.Width() || y < -(int)bitmap.Height()) return;
        uint_fast8_t firstPage = y >> 3;
        uint_fast8_t pageStart = y;
        uint_fast8_t pageEnd = ((y + 8) & 0xf8);

        uint_fast8_t bottom = y + bitmap.Height();
        if(bottom > 64)
            bottom = 64;
        uint_fast8_t lastPage = (bottom - 1) >> 3;
        uint_fast8_t maxX = min<Coord>(bitmap.Width(), 128 - x);
        uint_fast8_t line = 0;

        for(uint_fast8_t page = firstPage; page <= lastPage; page++)
        {
            if(page == lastPage)
                pageEnd = bottom;
            SetPage(page, x);
            uint_fast8_t pagePixels = pageEnd - pageStart;
            for(uint_fast8_t j = 0; j < maxX; j++)
            {
                uint8_t data = 0;
                for(uint_fast8_t i = 0; i < pagePixels; i++)
                {
                    data >>= 1;
                    if(bitmap(j, i + line) )
                        data |= 0x80;
                }
                WritePage(data, pageStart, pageEnd, x + j);
            }
            line += pagePixels;
            pageStart = pageEnd;
            pageEnd += 8;
        }
    }

    template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
            void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Flush()
    {

    }
} // namespace drivers
#endif
