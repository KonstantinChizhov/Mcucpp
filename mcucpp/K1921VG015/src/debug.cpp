#include <debug.h>
#include <atomic.h>
#include <arch.h>
#include <uart.h>
#include <string_util.h>

namespace Mcucpp
{
    void trap()
    {
        DisableInterrupts di;
        //wfi();
        while (true)
            ;
    }

#if defined MY_DEBUG && MY_DEBUG
    void debug_out(std::string_view str, unsigned value)
    {
        auto uart = Uart(0);
        uart.Write(str);
        uart.Write(' ');
        char buffer[9]={};

        char *ptr = UtoaHex(value, buffer);
        uart.Write(std::string_view{ptr, buffer});
        uart.Write('\r');
        uart.Write('\n');
    }

    void debug_init()
    {
        auto uart = Uart(0);
        uart.SelectPins(0, 0);
        uart.Init(115200);
    }
#endif

}

namespace std
{
#if DISABLED
    void __throw_out_of_range_fmt(const char *fmt, ...)
    {
        (void)fmt;
        Mcucpp::trap();
    }

    void __throw_length_error(const char *err)
    {
        (void)err;
        Mcucpp::trap();
    }

    void __throw_bad_function_call()
    {
        Mcucpp::trap();
    }
#endif
}