

#include <avr/io.h>


#include <iopins.h>
#include <delay.h>

#include <demo.h>


typedef IO::Pd7 Dbg;


//extern "C" void __cxa_pure_virtual()
//{
//	while(1);
//}


__attribute__((OS_main))
int main()
{
	Dbg::SetDirWrite();
	Setup();

	while(1)
    {
		Dbg::Set();
		Update();
		Dbg::Clear();
		Util::delay_ms<100, F_CPU>();
    }
}
