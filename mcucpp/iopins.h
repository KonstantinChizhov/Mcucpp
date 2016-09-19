//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
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

#include "iopin.h"
#include "ioports.h"
#if USE_SPLIT_PORT_CONFIGURATION == 8 

#define DECLARE_PORT_PINS(PORT_TYPE_NAME, PIN_NAME_PREFIX) \
	typedef TPin<PORT_TYPE_NAME, 0, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 0;\
	typedef TPin<PORT_TYPE_NAME, 1, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 1;\
	typedef TPin<PORT_TYPE_NAME, 2, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 2;\
	typedef TPin<PORT_TYPE_NAME, 3, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 3;\
	typedef TPin<PORT_TYPE_NAME, 4, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 4;\
	typedef TPin<PORT_TYPE_NAME, 5, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 5;\
	typedef TPin<PORT_TYPE_NAME, 6, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 6;\
	typedef TPin<PORT_TYPE_NAME, 7, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 7;\
	typedef TPin<PORT_TYPE_NAME, 8, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 8;\
	typedef TPin<PORT_TYPE_NAME, 9, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 9;\
	typedef TPin<PORT_TYPE_NAME, 10, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 10;\
	typedef TPin<PORT_TYPE_NAME, 11, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 11;\
	typedef TPin<PORT_TYPE_NAME, 12, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 12;\
	typedef TPin<PORT_TYPE_NAME, 13, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 13;\
	typedef TPin<PORT_TYPE_NAME, 14, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 14;\
	typedef TPin<PORT_TYPE_NAME, 15, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 15;\
\
	typedef InvertedPin<PORT_TYPE_NAME, 0, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 0 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 1, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 1 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 2, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 2 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 3, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 3 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 4, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 4 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 5, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 5 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 6, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 6 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 7, PORT_TYPE_NAME ## L> PIN_NAME_PREFIX ## 7 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 8, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 8 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 9, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 9 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 10, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 10 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 11, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 11 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 12, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 12 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 13, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 13 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 14, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 14 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 15, PORT_TYPE_NAME ## H> PIN_NAME_PREFIX ## 15 ## Inv;\

#else

#define DECLARE_PORT_PINS(PORT_TYPE_NAME, PIN_NAME_PREFIX) \
	typedef TPin<PORT_TYPE_NAME, 0> PIN_NAME_PREFIX ## 0;\
	typedef TPin<PORT_TYPE_NAME, 1> PIN_NAME_PREFIX ## 1;\
	typedef TPin<PORT_TYPE_NAME, 2> PIN_NAME_PREFIX ## 2;\
	typedef TPin<PORT_TYPE_NAME, 3> PIN_NAME_PREFIX ## 3;\
	typedef TPin<PORT_TYPE_NAME, 4> PIN_NAME_PREFIX ## 4;\
	typedef TPin<PORT_TYPE_NAME, 5> PIN_NAME_PREFIX ## 5;\
	typedef TPin<PORT_TYPE_NAME, 6> PIN_NAME_PREFIX ## 6;\
	typedef TPin<PORT_TYPE_NAME, 7> PIN_NAME_PREFIX ## 7;\
	typedef TPin<PORT_TYPE_NAME, 8> PIN_NAME_PREFIX ## 8;\
	typedef TPin<PORT_TYPE_NAME, 9> PIN_NAME_PREFIX ## 9;\
	typedef TPin<PORT_TYPE_NAME, 10> PIN_NAME_PREFIX ## 10;\
	typedef TPin<PORT_TYPE_NAME, 11> PIN_NAME_PREFIX ## 11;\
	typedef TPin<PORT_TYPE_NAME, 12> PIN_NAME_PREFIX ## 12;\
	typedef TPin<PORT_TYPE_NAME, 13> PIN_NAME_PREFIX ## 13;\
	typedef TPin<PORT_TYPE_NAME, 14> PIN_NAME_PREFIX ## 14;\
	typedef TPin<PORT_TYPE_NAME, 15> PIN_NAME_PREFIX ## 15;\
	typedef TPin<PORT_TYPE_NAME, 16> PIN_NAME_PREFIX ## 16;\
	typedef TPin<PORT_TYPE_NAME, 17> PIN_NAME_PREFIX ## 17;\
	typedef TPin<PORT_TYPE_NAME, 18> PIN_NAME_PREFIX ## 18;\
	typedef TPin<PORT_TYPE_NAME, 19> PIN_NAME_PREFIX ## 19;\
	typedef TPin<PORT_TYPE_NAME, 20> PIN_NAME_PREFIX ## 20;\
	typedef TPin<PORT_TYPE_NAME, 21> PIN_NAME_PREFIX ## 21;\
	typedef TPin<PORT_TYPE_NAME, 22> PIN_NAME_PREFIX ## 22;\
	typedef TPin<PORT_TYPE_NAME, 23> PIN_NAME_PREFIX ## 23;\
	typedef TPin<PORT_TYPE_NAME, 24> PIN_NAME_PREFIX ## 24;\
	typedef TPin<PORT_TYPE_NAME, 25> PIN_NAME_PREFIX ## 25;\
	typedef TPin<PORT_TYPE_NAME, 26> PIN_NAME_PREFIX ## 26;\
	typedef TPin<PORT_TYPE_NAME, 27> PIN_NAME_PREFIX ## 27;\
	typedef TPin<PORT_TYPE_NAME, 28> PIN_NAME_PREFIX ## 28;\
	typedef TPin<PORT_TYPE_NAME, 29> PIN_NAME_PREFIX ## 29;\
	typedef TPin<PORT_TYPE_NAME, 30> PIN_NAME_PREFIX ## 30;\
	typedef TPin<PORT_TYPE_NAME, 31> PIN_NAME_PREFIX ## 31;\
\
	typedef InvertedPin<PORT_TYPE_NAME, 0> PIN_NAME_PREFIX ## 0 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 1> PIN_NAME_PREFIX ## 1 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 2> PIN_NAME_PREFIX ## 2 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 3> PIN_NAME_PREFIX ## 3 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 4> PIN_NAME_PREFIX ## 4 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 5> PIN_NAME_PREFIX ## 5 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 6> PIN_NAME_PREFIX ## 6 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 7> PIN_NAME_PREFIX ## 7 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 8> PIN_NAME_PREFIX ## 8 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 9> PIN_NAME_PREFIX ## 9 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 10> PIN_NAME_PREFIX ## 10 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 11> PIN_NAME_PREFIX ## 11 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 12> PIN_NAME_PREFIX ## 12 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 13> PIN_NAME_PREFIX ## 13 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 14> PIN_NAME_PREFIX ## 14 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 15> PIN_NAME_PREFIX ## 15 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 16> PIN_NAME_PREFIX ## 16 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 17> PIN_NAME_PREFIX ## 17 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 18> PIN_NAME_PREFIX ## 18 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 19> PIN_NAME_PREFIX ## 19 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 20> PIN_NAME_PREFIX ## 20 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 21> PIN_NAME_PREFIX ## 21 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 22> PIN_NAME_PREFIX ## 22 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 23> PIN_NAME_PREFIX ## 23 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 24> PIN_NAME_PREFIX ## 24 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 25> PIN_NAME_PREFIX ## 25 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 26> PIN_NAME_PREFIX ## 26 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 27> PIN_NAME_PREFIX ## 27 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 28> PIN_NAME_PREFIX ## 28 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 29> PIN_NAME_PREFIX ## 29 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 30> PIN_NAME_PREFIX ## 30 ## Inv;\
	typedef InvertedPin<PORT_TYPE_NAME, 31> PIN_NAME_PREFIX ## 31 ## Inv;

#endif

namespace Mcucpp
{
namespace IO
{
	typedef TPin<NullPort, 0> NullPin;
	//Short pin definations

	#ifdef MCUCPP_HAS_PORTA
	DECLARE_PORT_PINS(Porta, Pa)
	#endif

	#ifdef MCUCPP_HAS_PORTB
	DECLARE_PORT_PINS(Portb, Pb)
	#endif

	#ifdef MCUCPP_HAS_PORTC
	DECLARE_PORT_PINS(Portc, Pc)
	#endif

	#ifdef MCUCPP_HAS_PORTD
	DECLARE_PORT_PINS(Portd, Pd)
	#endif

	#ifdef MCUCPP_HAS_PORTE
	DECLARE_PORT_PINS(Porte, Pe)
	#endif

	#ifdef MCUCPP_HAS_PORTF
	DECLARE_PORT_PINS(Portf, Pf)
	#endif

	#ifdef MCUCPP_HAS_PORTG
	DECLARE_PORT_PINS(Portg, Pg)
	#endif

	#ifdef MCUCPP_HAS_PORTH
	DECLARE_PORT_PINS(Porth, Ph)
	#endif
	
	#ifdef MCUCPP_HAS_PORTI
	DECLARE_PORT_PINS(Porti, Pi)
	#endif
	
	#ifdef MCUCPP_HAS_PORTJ
	DECLARE_PORT_PINS(Portj, Pj)
	#endif

	#ifdef MCUCPP_HAS_PORTK
	DECLARE_PORT_PINS(Portk, Pk)
	#endif

	#ifdef MCUCPP_HAS_PORTQ
	DECLARE_PORT_PINS(Portq, Pq)
	#endif

	#ifdef MCUCPP_HAS_PORTR
	DECLARE_PORT_PINS(Portr, Pr)
	#endif

	#ifdef MCUCPP_HAS_PORT0
	DECLARE_PORT_PINS(Port0, P0_)
	#endif

	#ifdef MCUCPP_HAS_PORT1
	DECLARE_PORT_PINS(Port1, P1_)
	#endif

	#ifdef MCUCPP_HAS_PORT2
	DECLARE_PORT_PINS(Port2, P2_)
	#endif

	#ifdef MCUCPP_HAS_PORT3
	DECLARE_PORT_PINS(Port3, P3_)
	#endif

	#ifdef MCUCPP_HAS_PORT4
	DECLARE_PORT_PINS(Port4, P4_)
	#endif

	#ifdef MCUCPP_HAS_PORT5
	DECLARE_PORT_PINS(Port5, P5_)
	#endif

	#ifdef MCUCPP_HAS_PORT6
	DECLARE_PORT_PINS(Port6, P6_)
	#endif

	#ifdef MCUCPP_HAS_PORT7
	DECLARE_PORT_PINS(Port7, P7_)
	#endif

	#ifdef MCUCPP_HAS_PORT8
	DECLARE_PORT_PINS(Port8, P8_)
	#endif

	#ifdef MCUCPP_HAS_PORT9
	DECLARE_PORT_PINS(Port9, P9_)
	#endif

	#ifdef MCUCPP_HAS_PORT10
	DECLARE_PORT_PINS(Port10, P10_)
	#endif

}
}
