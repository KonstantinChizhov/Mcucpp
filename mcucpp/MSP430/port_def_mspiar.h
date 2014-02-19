//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

/* Automatacaly generated header. */
/* This file contains definitions to determinate IO ports avalability for */
/* specified MSP430 device. */

#if defined(__MSP430C111__) | defined(__MSP430C1111__) | defined(__MSP430C112__) | defined(__MSP430C1121__) | defined(__MSP430E112__) | defined(__MSP430F110__) | \
	defined(__MSP430F1101__) | defined(__MSP430F1101A__) | defined(__MSP430F1111__) | defined(__MSP430F1111A__) | defined(__MSP430F112__) | defined(__MSP430F1121__) | \
	defined(__MSP430F1121A__) | defined(__MSP430F1122__) | defined(__MSP430F1132__) | defined(__MSP430F423__) | defined(__MSP430F425__) | defined(__MSP430F427__) | \
	defined(__MSP430F423A__) | defined(__MSP430F425A__) | defined(__MSP430F427A__) | defined(__MSP430FE423__) | defined(__MSP430FE425__) | defined(__MSP430FE427__) | \
	defined(__MSP430FE423A__) | defined(__MSP430FE425A__) | defined(__MSP430FE427A__) | defined(__MSP430FE4232__) | defined(__MSP430FE4242__) | defined(__MSP430FE4252__) | \
	defined(__MSP430FE4272__) | defined(__MSP430P112__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		/* Port 3 is not available */
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430C1331__) | defined(__MSP430C1351__) | defined(__MSP430C412__) | defined(__MSP430C413__) | defined(__MSP430F133__) | defined(__MSP430F135__) | \
	defined(__MSP430F147__) | defined(__MSP430F148__) | defined(__MSP430F149__) | defined(__MSP430F1471__) | defined(__MSP430F1481__) | defined(__MSP430F1491__) | \
	defined(__MSP430F155__) | defined(__MSP430F156__) | defined(__MSP430F157__) | defined(__MSP430F167__) | defined(__MSP430F168__) | defined(__MSP430F169__) | \
	defined(__MSP430F1610__) | defined(__MSP430F1611__) | defined(__MSP430F1612__) | defined(__MSP430F412__) | defined(__MSP430F413__) | defined(__MSP430F415__) | \
	defined(__MSP430F417__) | defined(__MSP430F435__) | defined(__MSP430F436__) | defined(__MSP430F437__) | defined(__MSP430F4351__) | defined(__MSP430F4361__) | \
	defined(__MSP430F4371__) | defined(__MSP430F4481__) | defined(__MSP430F4491__) | defined(__MSP430F447__) | defined(__MSP430F448__) | defined(__MSP430F449__) | \
	defined(__MSP430FW423__) | defined(__MSP430FW425__) | defined(__MSP430FW427__) | defined(__MSP430FG437__) | defined(__MSP430FG438__) | defined(__MSP430FG439__) | \
	defined(__MSP430F438__) | defined(__MSP430F439__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		MAKE_BASIC_PORT(P4, Port4, '4')
		MAKE_BASIC_PORT(P5, Port5, '5')
		MAKE_BASIC_PORT(P6, Port6, '6')
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430C311S__) | defined(__MSP430C312__) | defined(__MSP430C313__) | defined(__MSP430C314__) | defined(__MSP430C315__) | defined(__MSP430C323__) | \
	defined(__MSP430C325__) | defined(__MSP430E313__) | defined(__MSP430E315__) | defined(__MSP430E325__) | defined(__MSP430P313__) | defined(__MSP430P315__) | \
	defined(__MSP430P315S__) | defined(__MSP430P325__)
		MAKE_PORT_IE_NO_SEL(P0, Port0, '0')
		/* Port 1 is not available */
		/* Port 2 is not available */
		/* Port 3 is not available */
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430C336__) | defined(__MSP430C337__) | defined(__MSP430E337__) | defined(__MSP430P337__)
		MAKE_PORT_IE_NO_SEL(P0, Port0, '0')
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		MAKE_BASIC_PORT(P4, Port4, '4')
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430CG4616__) | defined(__MSP430CG4617__) | defined(__MSP430CG4618__) | defined(__MSP430CG4619__) | defined(__MSP430F46161__) | defined(__MSP430F46171__) | \
	defined(__MSP430F46181__) | defined(__MSP430F46191__) | defined(__MSP430F4616__) | defined(__MSP430F4617__) | defined(__MSP430F4618__) | defined(__MSP430F4619__) | \
	defined(__MSP430FG4616__) | defined(__MSP430FG4617__) | defined(__MSP430FG4618__) | defined(__MSP430FG4619__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		MAKE_BASIC_PORT(P4, Port4, '4')
		MAKE_BASIC_PORT(P5, Port5, '5')
		MAKE_BASIC_PORT(P6, Port6, '6')
		MAKE_BASIC_PORT(P7, Port7, '7')
		MAKE_BASIC_PORT(P8, Port8, '8')
		MAKE_BASIC_PORT(P9, Port9, '9')
		MAKE_BASIC_PORT(P10, Port10, '10')
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F122__) | defined(__MSP430F1222__) | defined(__MSP430F123__) | defined(__MSP430F1232__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F2001__) | defined(__MSP430F2011__) | defined(__MSP430F2002__) | defined(__MSP430F2012__) | defined(__MSP430F2003__) | defined(__MSP430F2013__) | \
	defined(__MSP430F2101__) | defined(__MSP430F2111__) | defined(__MSP430F2121__) | defined(__MSP430F2131__) | defined(__MSP430G2211__) | defined(__MSP430G2201__) | \
	defined(__MSP430G2111__) | defined(__MSP430G2101__) | defined(__MSP430G2001__) | defined(__MSP430G2231__) | defined(__MSP430G2221__) | defined(__MSP430G2131__) | \
	defined(__MSP430G2121__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		/* Port 3 is not available */
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F2112__) | defined(__MSP430F2122__) | defined(__MSP430F2132__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN_SEL2(P1, Port1, '1')
		MAKE_PORT_IE_REN_SEL2(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F2232__) | defined(__MSP430F2252__) | defined(__MSP430F2272__) | defined(__MSP430F2234__) | defined(__MSP430F2254__) | defined(__MSP430F2274__) | \
	defined(__MSP430F2330__) | defined(__MSP430F2350__) | defined(__MSP430F2370__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F233__) | defined(__MSP430F235__) | defined(__MSP430F2471__) | defined(__MSP430F2481__) | defined(__MSP430F2491__) | defined(__MSP430F5510__) | \
	defined(__MSP430F5513__) | defined(__MSP430F5514__) | defined(__MSP430F5522__) | defined(__MSP430F5524__) | defined(__MSP430F5526__) | defined(__MSP430F5528__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		MAKE_PORT_REN(P6, Port6, '6')
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F247__) | defined(__MSP430F248__) | defined(__MSP430F249__) | defined(__MSP430F2410__) | defined(__MSP430F2416__) | defined(__MSP430F2417__) | \
	defined(__MSP430F2418__) | defined(__MSP430F2419__) | defined(__MSP430F2616__) | defined(__MSP430F2617__) | defined(__MSP430F2618__) | defined(__MSP430F2619__) | \
	defined(__MSP430F5515__) | defined(__MSP430F5517__) | defined(__MSP430F5519__) | defined(__MSP430F5521__) | defined(__MSP430F5525__) | defined(__MSP430F5527__) | \
	defined(__MSP430F5529__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		MAKE_PORT_REN(P6, Port6, '6')
		MAKE_PORT_REN(P7, Port7, '7')
		MAKE_PORT_REN(P8, Port8, '8')
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F4132__) | defined(__MSP430F4152__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		MAKE_BASIC_PORT(P4, Port4, '4')
		MAKE_BASIC_PORT(P5, Port5, '5')
		MAKE_BASIC_PORT(P6, Port6, '6')
		MAKE_BASIC_PORT(P7, Port7, '7')
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F4783__) | defined(__MSP430F4793__) | defined(__MSP430F4784__) | defined(__MSP430F4794__) | defined(__MSP430F47126__) | defined(__MSP430F47127__) | \
	defined(__MSP430F47163__) | defined(__MSP430F47173__) | defined(__MSP430F47183__) | defined(__MSP430F47193__) | defined(__MSP430F47166__) | defined(__MSP430F47176__) | \
	defined(__MSP430F47186__) | defined(__MSP430F47196__) | defined(__MSP430F47167__) | defined(__MSP430F47177__) | defined(__MSP430F47187__) | defined(__MSP430F47197__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		/* Port 6 is not available */
		MAKE_PORT_REN(P7, Port7, '7')
		MAKE_PORT_REN(P8, Port8, '8')
		MAKE_PORT_REN(P9, Port9, '9')
		MAKE_PORT_REN(P10, Port10, '10')
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F4250__) | defined(__MSP430F4260__) | defined(__MSP430F4270__) | defined(__MSP430FG4250__) | defined(__MSP430FG4260__) | defined(__MSP430FG4270__)
		/* Port 0 is not available */
		MAKE_PORT_IE(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		/* Port 3 is not available */
		/* Port 4 is not available */
		MAKE_BASIC_PORT(P5, Port5, '5')
		MAKE_BASIC_PORT(P6, Port6, '6')
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F477__) | defined(__MSP430F478__) | defined(__MSP430F479__) | defined(__MSP430FG477__) | defined(__MSP430FG478__) | defined(__MSP430FG479__)
		/* Port 0 is not available */
		MAKE_PORT_IE_SEL2(P1, Port1, '1')
		MAKE_PORT_IE(P2, Port2, '2')
		MAKE_BASIC_PORT(P3, Port3, '3')
		MAKE_BASIC_PORT(P4, Port4, '4')
		MAKE_BASIC_PORT(P5, Port5, '5')
		MAKE_BASIC_PORT(P6, Port6, '6')
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__XMS430F5438__) | defined(__MSP430F5418__) | defined(__MSP430F5419__) | defined(__MSP430F5435__) | defined(__MSP430F5436__) | defined(__MSP430F5437__) | \
	defined(__MSP430F5438__) | defined(__MSP430F5418A__) | defined(__MSP430F5419A__) | defined(__MSP430F5435A__) | defined(__MSP430F5436A__) | defined(__MSP430F5437A__) | \
	defined(__MSP430F5438A__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		MAKE_PORT_REN(P6, Port6, '6')
		MAKE_PORT_REN(P7, Port7, '7')
		MAKE_PORT_REN(P8, Port8, '8')
		MAKE_PORT_REN(P9, Port9, '9')
		MAKE_PORT_REN(P10, Port10, '10')
		MAKE_PORT_REN(P11, Port11, '11')
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__CC430F5133__) | defined(__CC430F5135__) | defined(__CC430F5137__) | defined(__CC430F6125__) | defined(__CC430F6126__) | defined(__CC430F6127__) | \
	defined(__CC430F6135__) | defined(__CC430F6137__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		MAKE_PORT_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F6638__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_IE_REN(P3, Port3, '3')
		MAKE_PORT_IE_REN(P4, Port4, '4')
		MAKE_PORT_REN(P5, Port5, '5')
		MAKE_PORT_REN(P6, Port6, '6')
		MAKE_PORT_REN(P7, Port7, '7')
		MAKE_PORT_REN(P8, Port8, '8')
		MAKE_PORT_REN(P9, Port9, '9')
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430L092__) | defined(__MSP430C091__) | defined(__MSP430C092__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN_NO_SEL(P1, Port1, '1')
		MAKE_PORT_IE_REN_NO_SEL(P2, Port2, '2')
		/* Port 3 is not available */
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */

#elif defined(__MSP430F5131__) | defined(__MSP430F5151__) | defined(__MSP430F5171__) | defined(__MSP430F5132__) | defined(__MSP430F5152__) | defined(__MSP430F5172__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		/* Port 4 is not available */
		/* Port 5 is not available */
		/* Port 6 is not available */
		/* Port 7 is not available */
		/* Port 8 is not available */
		/* Port 9 is not available */
		/* Port 10 is not available */
		/* Port 11 is not available */
		/* Port 12 is not available */
		/* Port 13 is not available */
		/* Port 14 is not available */


#else

#warning 'Unknown MSP430 device. Using minimal port functionality.'

#if defined(__MSP430_HAS_PORT0__)  ||  defined(__MSP430_HAS_PORT0_R__)
		MAKE_BASIC_PORT_NO_SEL(P0, Port0, '0')
#endif

#if defined(__MSP430_HAS_PORT1__)  ||  defined(__MSP430_HAS_PORT1_R__)
		MAKE_BASIC_PORT_NO_SEL(P1, Port1, '1')
#endif

#if defined(__MSP430_HAS_PORT2__)  ||  defined(__MSP430_HAS_PORT2_R__)
		MAKE_BASIC_PORT_NO_SEL(P2, Port2, '2')
#endif

#if defined(__MSP430_HAS_PORT3__)  ||  defined(__MSP430_HAS_PORT3_R__)
		MAKE_BASIC_PORT_NO_SEL(P3, Port3, '3')
#endif

#if defined(__MSP430_HAS_PORT4__)  ||  defined(__MSP430_HAS_PORT4_R__)
		MAKE_BASIC_PORT_NO_SEL(P4, Port4, '4')
#endif

#if defined(__MSP430_HAS_PORT5__)  ||  defined(__MSP430_HAS_PORT5_R__)
		MAKE_BASIC_PORT_NO_SEL(P5, Port5, '5')
#endif

#if defined(__MSP430_HAS_PORT6__)  ||  defined(__MSP430_HAS_PORT6_R__)
		MAKE_BASIC_PORT_NO_SEL(P6, Port6, '6')
#endif

#if defined(__MSP430_HAS_PORT7__)  ||  defined(__MSP430_HAS_PORT7_R__)
		MAKE_BASIC_PORT_NO_SEL(P7, Port7, '7')
#endif

#if defined(__MSP430_HAS_PORT8__)  ||  defined(__MSP430_HAS_PORT8_R__)
		MAKE_BASIC_PORT_NO_SEL(P8, Port8, '8')
#endif

#if defined(__MSP430_HAS_PORT9__)  ||  defined(__MSP430_HAS_PORT9_R__)
		MAKE_BASIC_PORT_NO_SEL(P9, Port9, '9')
#endif

#if defined(__MSP430_HAS_PORT10__)  ||  defined(__MSP430_HAS_PORT10_R__)
		MAKE_BASIC_PORT_NO_SEL(P10, Port10, '10')
#endif

#if defined(__MSP430_HAS_PORT11__)  ||  defined(__MSP430_HAS_PORT11_R__)
		MAKE_BASIC_PORT_NO_SEL(P11, Port11, '11')
#endif

#if defined(__MSP430_HAS_PORT12__)  ||  defined(__MSP430_HAS_PORT12_R__)
		MAKE_BASIC_PORT_NO_SEL(P12, Port12, '12')
#endif

#if defined(__MSP430_HAS_PORT13__)  ||  defined(__MSP430_HAS_PORT13_R__)
		MAKE_BASIC_PORT_NO_SEL(P13, Port13, '13')
#endif

#if defined(__MSP430_HAS_PORT14__)  ||  defined(__MSP430_HAS_PORT14_R__)
		MAKE_BASIC_PORT_NO_SEL(P14, Port14, '14')
#endif

#endif

