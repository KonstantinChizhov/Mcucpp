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

#if defined(__CC430_5133__) | defined(__CC430X5133__) | defined(__CC430F5133__) | defined(__CC430_5135__) | defined(__CC430X5135__) | defined(__CC430F5135__) | \
	defined(__CC430_5137__) | defined(__CC430X5137__) | defined(__CC430F5137__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN(P1, Port1, '1')
		MAKE_PORT_IE_REN(P2, Port2, '2')
		MAKE_PORT_REN(P3, Port3, '3')
		/* Port 4 is not available */
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

#elif defined(__CC430_6125__) | defined(__CC430X6125__) | defined(__CC430F6125__) | defined(__CC430_6126__) | defined(__CC430X6126__) | defined(__CC430F6126__) | \
	defined(__CC430_6127__) | defined(__CC430X6127__) | defined(__CC430F6127__) | defined(__CC430_6135__) | defined(__CC430X6135__) | defined(__CC430F6135__) | \
	defined(__CC430_6137__) | defined(__CC430X6137__) | defined(__CC430F6137__)
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

#elif defined(__MSP430_091__) | defined(__MSP430X091__) | defined(__MSP430C091__) | defined(__MSP430L092__) | defined(__MSP430C092__) | defined(__MSP430_5720__) | \
	defined(__MSP430X5720__) | defined(__MSP430FR5720__) | defined(__MSP430_5728__) | defined(__MSP430X5728__) | defined(__MSP430FR5728__) | defined(__MSP430_5730__) | \
	defined(__MSP430X5730__) | defined(__MSP430FR5730__) | defined(__MSP430_5738__) | defined(__MSP430X5738__) | defined(__MSP430FR5738__)
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

#elif defined(__MSP430_110__) | defined(__MSP430X110__) | defined(__MSP430F110__) | defined(__MSP430F1101__) | defined(__MSP430F1101A__) | defined(__MSP430_111__) | \
	defined(__MSP430X111__) | defined(__MSP430C111__) | defined(__MSP430F1111__) | defined(__MSP430F1111A__) | defined(__MSP430C1111__) | defined(__MSP430E112__) | \
	defined(__MSP430F112__) | defined(__MSP430P112__) | defined(__MSP430C112__) | defined(__MSP430F1121__) | defined(__MSP430F1121A__) | defined(__MSP430C1121__) | \
	defined(__MSP430_1122__) | defined(__MSP430X1122__) | defined(__MSP430F1122__) | defined(__MSP430_1132__) | defined(__MSP430X1132__) | defined(__MSP430F1132__) | \
	defined(__MSP430F423__) | defined(__MSP430FE423__) | defined(__MSP430F423A__) | defined(__MSP430FE423A__) | defined(__MSP430_4232__) | defined(__MSP430X4232__) | \
	defined(__MSP430FE4232__) | defined(__MSP430_4242__) | defined(__MSP430X4242__) | defined(__MSP430FE4242__) | defined(__MSP430FE425__) | defined(__MSP430FE425A__) | \
	defined(__MSP430F425A__) | defined(__MSP430F425__) | defined(__MSP430_4252__) | defined(__MSP430X4252__) | defined(__MSP430FE4252__) | defined(__MSP430F427A__) | \
	defined(__MSP430FE427__) | defined(__MSP430F427__) | defined(__MSP430FE427A__) | defined(__MSP430_4272__) | defined(__MSP430X4272__) | defined(__MSP430FE4272__)
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

#elif defined(__MSP430_122__) | defined(__MSP430X122__) | defined(__MSP430F122__) | defined(__MSP430_1222__) | defined(__MSP430X1222__) | defined(__MSP430F1222__) | \
	defined(__MSP430_123__) | defined(__MSP430X123__) | defined(__MSP430F123__) | defined(__MSP430_1232__) | defined(__MSP430X1232__) | defined(__MSP430F1232__)
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

#elif defined(__MSP430_133__) | defined(__MSP430X133__) | defined(__MSP430F133__) | defined(__MSP430_1331__) | defined(__MSP430X1331__) | defined(__MSP430C1331__) | \
	defined(__MSP430_135__) | defined(__MSP430X135__) | defined(__MSP430F135__) | defined(__MSP430_1351__) | defined(__MSP430X1351__) | defined(__MSP430C1351__) | \
	defined(__MSP430_147__) | defined(__MSP430X147__) | defined(__MSP430F147__) | defined(__MSP430_1471__) | defined(__MSP430X1471__) | defined(__MSP430F1471__) | \
	defined(__MSP430_148__) | defined(__MSP430X148__) | defined(__MSP430F148__) | defined(__MSP430_1481__) | defined(__MSP430X1481__) | defined(__MSP430F1481__) | \
	defined(__MSP430_149__) | defined(__MSP430X149__) | defined(__MSP430F149__) | defined(__MSP430_1491__) | defined(__MSP430X1491__) | defined(__MSP430F1491__) | \
	defined(__MSP430_155__) | defined(__MSP430X155__) | defined(__MSP430F155__) | defined(__MSP430_156__) | defined(__MSP430X156__) | defined(__MSP430F156__) | \
	defined(__MSP430_157__) | defined(__MSP430X157__) | defined(__MSP430F157__) | defined(__MSP430_1610__) | defined(__MSP430X1610__) | defined(__MSP430F1610__) | \
	defined(__MSP430_1611__) | defined(__MSP430X1611__) | defined(__MSP430F1611__) | defined(__MSP430_1612__) | defined(__MSP430X1612__) | defined(__MSP430F1612__) | \
	defined(__MSP430_167__) | defined(__MSP430X167__) | defined(__MSP430F167__) | defined(__MSP430_168__) | defined(__MSP430X168__) | defined(__MSP430F168__) | \
	defined(__MSP430_169__) | defined(__MSP430X169__) | defined(__MSP430F169__) | defined(__MSP430F412__) | defined(__MSP430C412__) | defined(__MSP430F413__) | \
	defined(__MSP430C413__) | defined(__MSP430_415__) | defined(__MSP430X415__) | defined(__MSP430F415__) | defined(__MSP430_417__) | defined(__MSP430X417__) | \
	defined(__MSP430F417__) | defined(__MSP430FW423__) | defined(__MSP430FW425__) | defined(__MSP430FW427__) | defined(__MSP430_428__) | defined(__MSP430X428__) | \
	defined(__MSP430FW428__) | defined(__MSP430_429__) | defined(__MSP430X429__) | defined(__MSP430FW429__) | defined(__MSP430_435__) | defined(__MSP430X435__) | \
	defined(__MSP430F435__) | defined(__MSP430_4351__) | defined(__MSP430X4351__) | defined(__MSP430F4351__) | defined(__MSP430_436__) | defined(__MSP430X436__) | \
	defined(__MSP430F436__) | defined(__MSP430_4361__) | defined(__MSP430X4361__) | defined(__MSP430F4361__) | defined(__MSP430FG437__) | defined(__MSP430F437__) | \
	defined(__MSP430_4371__) | defined(__MSP430X4371__) | defined(__MSP430F4371__) | defined(__MSP430F438__) | defined(__MSP430FG438__) | defined(__MSP430F439__) | \
	defined(__MSP430FG439__) | defined(__MSP430_447__) | defined(__MSP430X447__) | defined(__MSP430F447__) | defined(__MSP430_448__) | defined(__MSP430X448__) | \
	defined(__MSP430F448__) | defined(__MSP430_4481__) | defined(__MSP430X4481__) | defined(__MSP430F4481__) | defined(__MSP430_449__) | defined(__MSP430X449__) | \
	defined(__MSP430F449__) | defined(__MSP430_4491__) | defined(__MSP430X4491__) | defined(__MSP430F4491__)
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

#elif defined(__MSP430G2001__) | defined(__MSP430F2001__) | defined(__MSP430_2002__) | defined(__MSP430X2002__) | defined(__MSP430F2002__) | defined(__MSP430_2003__) | \
	defined(__MSP430X2003__) | defined(__MSP430F2003__) | defined(__MSP430_2011__) | defined(__MSP430X2011__) | defined(__MSP430F2011__) | defined(__MSP430_2012__) | \
	defined(__MSP430X2012__) | defined(__MSP430F2012__) | defined(__MSP430_2013__) | defined(__MSP430X2013__) | defined(__MSP430F2013__) | defined(__MSP430G2101__) | \
	defined(__MSP430F2101__) | defined(__MSP430G2111__) | defined(__MSP430F2111__) | defined(__MSP430F2121__) | defined(__MSP430G2121__) | defined(__MSP430F2131__) | \
	defined(__MSP430G2131__) | defined(__MSP430_2201__) | defined(__MSP430X2201__) | defined(__MSP430G2201__) | defined(__MSP430_2211__) | defined(__MSP430X2211__) | \
	defined(__MSP430G2211__) | defined(__MSP430_2221__) | defined(__MSP430X2221__) | defined(__MSP430G2221__) | defined(__MSP430_2231__) | defined(__MSP430X2231__) | \
	defined(__MSP430G2231__)
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

#elif defined(__MSP430_2102__) | defined(__MSP430X2102__) | defined(__MSP430G2102__) | defined(__MSP430G2112__) | defined(__MSP430G2132__) | defined(__MSP430_2152__) | \
	defined(__MSP430X2152__) | defined(__MSP430G2152__) | defined(__MSP430_2202__) | defined(__MSP430X2202__) | defined(__MSP430G2202__) | defined(__MSP430_221__) | \
	defined(__MSP430X221__) | defined(__MSP430AFE221__) | defined(__MSP430_2212__) | defined(__MSP430X2212__) | defined(__MSP430G2212__) | defined(__MSP430_222__) | \
	defined(__MSP430X222__) | defined(__MSP430AFE222__) | defined(__MSP430_223__) | defined(__MSP430X223__) | defined(__MSP430AFE223__) | defined(__MSP430G2232__) | \
	defined(__MSP430G2252__) | defined(__MSP430_2302__) | defined(__MSP430X2302__) | defined(__MSP430G2302__) | defined(__MSP430_231__) | defined(__MSP430X231__) | \
	defined(__MSP430AFE231__) | defined(__MSP430_2312__) | defined(__MSP430X2312__) | defined(__MSP430G2312__) | defined(__MSP430_232__) | defined(__MSP430X232__) | \
	defined(__MSP430AFE232__) | defined(__MSP430AFE233__) | defined(__MSP430_2332__) | defined(__MSP430X2332__) | defined(__MSP430G2332__) | defined(__MSP430_2352__) | \
	defined(__MSP430X2352__) | defined(__MSP430G2352__) | defined(__MSP430_2402__) | defined(__MSP430X2402__) | defined(__MSP430G2402__) | defined(__MSP430_2412__) | \
	defined(__MSP430X2412__) | defined(__MSP430G2412__) | defined(__MSP430_2432__) | defined(__MSP430X2432__) | defined(__MSP430G2432__) | defined(__MSP430_2452__) | \
	defined(__MSP430X2452__) | defined(__MSP430G2452__) | defined(__MSP430_251__) | defined(__MSP430X251__) | defined(__MSP430AFE251__) | defined(__MSP430_252__) | \
	defined(__MSP430X252__) | defined(__MSP430AFE252__) | defined(__MSP430_253__) | defined(__MSP430X253__) | defined(__MSP430AFE253__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN_SEL2(P1, Port1, '1')
		MAKE_PORT_IE_REN_SEL2(P2, Port2, '2')
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

#elif defined(__MSP430F2112__) | defined(__MSP430_2122__) | defined(__MSP430X2122__) | defined(__MSP430F2122__) | defined(__MSP430F2132__)
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

#elif defined(__MSP430_2113__) | defined(__MSP430X2113__) | defined(__MSP430G2113__) | defined(__MSP430_2153__) | defined(__MSP430X2153__) | defined(__MSP430G2153__) | \
	defined(__MSP430_2203__) | defined(__MSP430X2203__) | defined(__MSP430G2203__) | defined(__MSP430_2213__) | defined(__MSP430X2213__) | defined(__MSP430G2213__) | \
	defined(__MSP430_2233__) | defined(__MSP430X2233__) | defined(__MSP430G2233__) | defined(__MSP430_2253__) | defined(__MSP430X2253__) | defined(__MSP430G2253__) | \
	defined(__MSP430_2303__) | defined(__MSP430X2303__) | defined(__MSP430G2303__) | defined(__MSP430_2313__) | defined(__MSP430X2313__) | defined(__MSP430G2313__) | \
	defined(__MSP430_2333__) | defined(__MSP430X2333__) | defined(__MSP430G2333__) | defined(__MSP430_2353__) | defined(__MSP430X2353__) | defined(__MSP430G2353__) | \
	defined(__MSP430_2403__) | defined(__MSP430X2403__) | defined(__MSP430G2403__) | defined(__MSP430_2413__) | defined(__MSP430X2413__) | defined(__MSP430G2413__) | \
	defined(__MSP430_2433__) | defined(__MSP430X2433__) | defined(__MSP430G2433__) | defined(__MSP430_2453__) | defined(__MSP430X2453__) | defined(__MSP430G2453__) | \
	defined(__MSP430_2513__) | defined(__MSP430X2513__) | defined(__MSP430G2513__) | defined(__MSP430_2533__) | defined(__MSP430X2533__) | defined(__MSP430G2533__) | \
	defined(__MSP430_2553__) | defined(__MSP430X2553__) | defined(__MSP430G2553__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN_SEL2(P1, Port1, '1')
		MAKE_PORT_IE_REN_SEL2(P2, Port2, '2')
		MAKE_PORT_REN_SEL2(P3, Port3, '3')
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

#elif defined(__MSP430F2232__) | defined(__MSP430_2234__) | defined(__MSP430X2234__) | defined(__MSP430F2234__) | defined(__MSP430F2252__) | defined(__MSP430_2254__) | \
	defined(__MSP430X2254__) | defined(__MSP430F2254__) | defined(__MSP430_2272__) | defined(__MSP430X2272__) | defined(__MSP430F2272__) | defined(__MSP430_2274__) | \
	defined(__MSP430X2274__) | defined(__MSP430F2274__) | defined(__MSP430_2330__) | defined(__MSP430X2330__) | defined(__MSP430F2330__) | defined(__MSP430_2350__) | \
	defined(__MSP430X2350__) | defined(__MSP430F2350__) | defined(__MSP430_2370__) | defined(__MSP430X2370__) | defined(__MSP430F2370__)
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

#elif defined(__MSP430F233__) | defined(__MSP430_235__) | defined(__MSP430X235__) | defined(__MSP430F235__) | defined(__MSP430_2410__) | defined(__MSP430X2410__) | \
	defined(__MSP430F2410__) | defined(__MSP430_247__) | defined(__MSP430X247__) | defined(__MSP430F247__) | defined(__MSP430_2471__) | defined(__MSP430X2471__) | \
	defined(__MSP430F2471__) | defined(__MSP430_248__) | defined(__MSP430X248__) | defined(__MSP430F248__) | defined(__MSP430_2481__) | defined(__MSP430X2481__) | \
	defined(__MSP430F2481__) | defined(__MSP430_249__) | defined(__MSP430X249__) | defined(__MSP430F249__) | defined(__MSP430_2491__) | defined(__MSP430X2491__) | \
	defined(__MSP430F2491__) | defined(__MSP430_5304__) | defined(__MSP430X5304__) | defined(__MSP430F5304__) | defined(__MSP430_5308__) | defined(__MSP430X5308__) | \
	defined(__MSP430F5308__) | defined(__MSP430_5309__) | defined(__MSP430X5309__) | defined(__MSP430F5309__) | defined(__MSP430_5310__) | defined(__MSP430X5310__) | \
	defined(__MSP430F5310__) | defined(__MSP430_5324__) | defined(__MSP430X5324__) | defined(__MSP430F5324__) | defined(__MSP430_5326__) | defined(__MSP430X5326__) | \
	defined(__MSP430F5326__) | defined(__MSP430_5328__) | defined(__MSP430X5328__) | defined(__MSP430F5328__) | defined(__MSP430_5340__) | defined(__MSP430X5340__) | \
	defined(__MSP430F5340__) | defined(__MSP430_5341__) | defined(__MSP430X5341__) | defined(__MSP430F5341__) | defined(__MSP430_5342__) | defined(__MSP430X5342__) | \
	defined(__MSP430F5342__) | defined(__MSP430_5500__) | defined(__MSP430X5500__) | defined(__MSP430F5500__) | defined(__MSP430_5501__) | defined(__MSP430X5501__) | \
	defined(__MSP430F5501__) | defined(__MSP430_5502__) | defined(__MSP430X5502__) | defined(__MSP430F5502__) | defined(__MSP430_5503__) | defined(__MSP430X5503__) | \
	defined(__MSP430F5503__) | defined(__MSP430_5504__) | defined(__MSP430X5504__) | defined(__MSP430F5504__) | defined(__MSP430_5505__) | defined(__MSP430X5505__) | \
	defined(__MSP430F5505__) | defined(__MSP430_5506__) | defined(__MSP430X5506__) | defined(__MSP430F5506__) | defined(__MSP430_5507__) | defined(__MSP430X5507__) | \
	defined(__MSP430F5507__) | defined(__MSP430_5508__) | defined(__MSP430X5508__) | defined(__MSP430F5508__) | defined(__MSP430_5509__) | defined(__MSP430X5509__) | \
	defined(__MSP430F5509__) | defined(__MSP430_5510__) | defined(__MSP430X5510__) | defined(__MSP430F5510__) | defined(__MSP430_5513__) | defined(__MSP430X5513__) | \
	defined(__MSP430F5513__) | defined(__MSP430_5514__) | defined(__MSP430X5514__) | defined(__MSP430F5514__) | defined(__MSP430_5522__) | defined(__MSP430X5522__) | \
	defined(__MSP430F5522__) | defined(__MSP430_5524__) | defined(__MSP430X5524__) | defined(__MSP430F5524__) | defined(__MSP430_5526__) | defined(__MSP430X5526__) | \
	defined(__MSP430F5526__) | defined(__MSP430_5528__) | defined(__MSP430X5528__) | defined(__MSP430F5528__)
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

#elif defined(__MSP430_2416__) | defined(__MSP430X2416__) | defined(__MSP430F2416__) | defined(__MSP430_2417__) | defined(__MSP430X2417__) | defined(__MSP430F2417__) | \
	defined(__MSP430_2418__) | defined(__MSP430X2418__) | defined(__MSP430F2418__) | defined(__MSP430_2419__) | defined(__MSP430X2419__) | defined(__MSP430F2419__) | \
	defined(__MSP430_2616__) | defined(__MSP430X2616__) | defined(__MSP430F2616__) | defined(__MSP430_2617__) | defined(__MSP430X2617__) | defined(__MSP430F2617__) | \
	defined(__MSP430_2618__) | defined(__MSP430X2618__) | defined(__MSP430F2618__) | defined(__MSP430_2619__) | defined(__MSP430X2619__) | defined(__MSP430F2619__) | \
	defined(__MSP430_5325__) | defined(__MSP430X5325__) | defined(__MSP430F5325__) | defined(__MSP430_5327__) | defined(__MSP430X5327__) | defined(__MSP430F5327__) | \
	defined(__MSP430_5329__) | defined(__MSP430X5329__) | defined(__MSP430F5329__) | defined(__MSP430_5515__) | defined(__MSP430X5515__) | defined(__MSP430F5515__) | \
	defined(__MSP430_5517__) | defined(__MSP430X5517__) | defined(__MSP430F5517__) | defined(__MSP430_5519__) | defined(__MSP430X5519__) | defined(__MSP430F5519__) | \
	defined(__MSP430_5521__) | defined(__MSP430X5521__) | defined(__MSP430F5521__) | defined(__MSP430_5525__) | defined(__MSP430X5525__) | defined(__MSP430F5525__) | \
	defined(__MSP430_5527__) | defined(__MSP430X5527__) | defined(__MSP430F5527__) | defined(__MSP430_5529__) | defined(__MSP430X5529__) | defined(__MSP430F5529__)
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

#elif defined(__MSP430_311__) | defined(__MSP430X311__) | defined(__MSP430C311S__) | defined(__MSP430_312__) | defined(__MSP430X312__) | defined(__MSP430C312__) | \
	defined(__MSP430C313__) | defined(__MSP430P313__) | defined(__MSP430E313__) | defined(__MSP430_314__) | defined(__MSP430X314__) | defined(__MSP430C314__) | \
	defined(__MSP430P315S__) | defined(__MSP430E315__) | defined(__MSP430P315__) | defined(__MSP430C315__) | defined(__MSP430_323__) | defined(__MSP430X323__) | \
	defined(__MSP430C323__) | defined(__MSP430P325__) | defined(__MSP430E325__) | defined(__MSP430C325__)
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

#elif defined(__MSP430_336__) | defined(__MSP430X336__) | defined(__MSP430C336__) | defined(__MSP430P337__) | defined(__MSP430C337__) | defined(__MSP430E337__)
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

#elif defined(__MSP430_4132__) | defined(__MSP430X4132__) | defined(__MSP430F4132__) | defined(__MSP430_4152__) | defined(__MSP430X4152__) | defined(__MSP430F4152__)
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

#elif defined(__MSP430FG4250__) | defined(__MSP430F4250__) | defined(__MSP430FG4260__) | defined(__MSP430F4260__) | defined(__MSP430F4270__) | defined(__MSP430FG4270__)
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

#elif defined(__MSP430CG4616__) | defined(__MSP430F4616__) | defined(__MSP430FG4616__) | defined(__MSP430_46161__) | defined(__MSP430X46161__) | defined(__MSP430F46161__) | \
	defined(__MSP430F4617__) | defined(__MSP430CG4617__) | defined(__MSP430FG4617__) | defined(__MSP430_46171__) | defined(__MSP430X46171__) | defined(__MSP430F46171__) | \
	defined(__MSP430FG4618__) | defined(__MSP430CG4618__) | defined(__MSP430F4618__) | defined(__MSP430_46181__) | defined(__MSP430X46181__) | defined(__MSP430F46181__) | \
	defined(__MSP430FG4619__) | defined(__MSP430F4619__) | defined(__MSP430CG4619__) | defined(__MSP430_46191__) | defined(__MSP430X46191__) | defined(__MSP430F46191__)
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

#elif defined(__MSP430_47126__) | defined(__MSP430X47126__) | defined(__MSP430F47126__) | defined(__MSP430_47127__) | defined(__MSP430X47127__) | defined(__MSP430F47127__) | \
	defined(__MSP430_47163__) | defined(__MSP430X47163__) | defined(__MSP430F47163__) | defined(__MSP430_47166__) | defined(__MSP430X47166__) | defined(__MSP430F47166__) | \
	defined(__MSP430_47167__) | defined(__MSP430X47167__) | defined(__MSP430F47167__) | defined(__MSP430_47173__) | defined(__MSP430X47173__) | defined(__MSP430F47173__) | \
	defined(__MSP430_47176__) | defined(__MSP430X47176__) | defined(__MSP430F47176__) | defined(__MSP430_47177__) | defined(__MSP430X47177__) | defined(__MSP430F47177__) | \
	defined(__MSP430_47183__) | defined(__MSP430X47183__) | defined(__MSP430F47183__) | defined(__MSP430_47186__) | defined(__MSP430X47186__) | defined(__MSP430F47186__) | \
	defined(__MSP430_47187__) | defined(__MSP430X47187__) | defined(__MSP430F47187__) | defined(__MSP430_47193__) | defined(__MSP430X47193__) | defined(__MSP430F47193__) | \
	defined(__MSP430_47196__) | defined(__MSP430X47196__) | defined(__MSP430F47196__) | defined(__MSP430_47197__) | defined(__MSP430X47197__) | defined(__MSP430F47197__) | \
	defined(__MSP430_4783__) | defined(__MSP430X4783__) | defined(__MSP430F4783__) | defined(__MSP430_4784__) | defined(__MSP430X4784__) | defined(__MSP430F4784__) | \
	defined(__MSP430_4793__) | defined(__MSP430X4793__) | defined(__MSP430F4793__) | defined(__MSP430_4794__) | defined(__MSP430X4794__) | defined(__MSP430F4794__)
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

#elif defined(__MSP430FG477__) | defined(__MSP430F477__) | defined(__MSP430F478__) | defined(__MSP430FG478__) | defined(__MSP430F479__) | defined(__MSP430FG479__)
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

#elif defined(__MSP430_5131__) | defined(__MSP430X5131__) | defined(__MSP430F5131__) | defined(__MSP430_5132__) | defined(__MSP430X5132__) | defined(__MSP430F5132__) | \
	defined(__MSP430_5151__) | defined(__MSP430X5151__) | defined(__MSP430F5151__) | defined(__MSP430_5152__) | defined(__MSP430X5152__) | defined(__MSP430F5152__) | \
	defined(__MSP430_5171__) | defined(__MSP430X5171__) | defined(__MSP430F5171__) | defined(__MSP430_5172__) | defined(__MSP430X5172__) | defined(__MSP430F5172__)
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

#elif defined(__MSP430_5190__) | defined(__MSP430X5190__) | defined(__MSP430BT5190__) | defined(__MSP430F5418A__) | defined(__MSP430F5418__) | defined(__MSP430F5419A__) | \
	defined(__MSP430F5419__) | defined(__MSP430F5435__) | defined(__MSP430F5435A__) | defined(__MSP430F5436__) | defined(__MSP430F5436A__) | defined(__MSP430F5437__) | \
	defined(__MSP430F5437A__) | defined(__MSP430F5438A__) | defined(__MSP430F5438__)
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

#elif defined(__MSP430_5630__) | defined(__MSP430X5630__) | defined(__MSP430F5630__) | defined(__MSP430_5631__) | defined(__MSP430X5631__) | defined(__MSP430F5631__) | \
	defined(__MSP430_5632__) | defined(__MSP430X5632__) | defined(__MSP430F5632__) | defined(__MSP430_5633__) | defined(__MSP430X5633__) | defined(__MSP430F5633__) | \
	defined(__MSP430_5634__) | defined(__MSP430X5634__) | defined(__MSP430F5634__) | defined(__MSP430_5635__) | defined(__MSP430X5635__) | defined(__MSP430F5635__) | \
	defined(__MSP430_5636__) | defined(__MSP430X5636__) | defined(__MSP430F5636__) | defined(__MSP430_5637__) | defined(__MSP430X5637__) | defined(__MSP430F5637__) | \
	defined(__MSP430_5638__) | defined(__MSP430X5638__) | defined(__MSP430F5638__) | defined(__MSP430_6630__) | defined(__MSP430X6630__) | defined(__MSP430F6630__) | \
	defined(__MSP430_6631__) | defined(__MSP430X6631__) | defined(__MSP430F6631__) | defined(__MSP430_6632__) | defined(__MSP430X6632__) | defined(__MSP430F6632__) | \
	defined(__MSP430_6633__) | defined(__MSP430X6633__) | defined(__MSP430F6633__) | defined(__MSP430_6634__) | defined(__MSP430X6634__) | defined(__MSP430F6634__) | \
	defined(__MSP430_6635__) | defined(__MSP430X6635__) | defined(__MSP430F6635__) | defined(__MSP430_6636__) | defined(__MSP430X6636__) | defined(__MSP430F6636__) | \
	defined(__MSP430_6637__) | defined(__MSP430X6637__) | defined(__MSP430F6637__) | defined(__MSP430_6638__) | defined(__MSP430X6638__) | defined(__MSP430F6638__)
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

#elif defined(__MSP430_5725__) | defined(__MSP430X5725__) | defined(__MSP430FR5725__) | defined(__MSP430_5729__) | defined(__MSP430X5729__) | defined(__MSP430FR5729__) | \
	defined(__MSP430_5735__) | defined(__MSP430X5735__) | defined(__MSP430FR5735__) | defined(__MSP430_5739__) | defined(__MSP430X5739__) | defined(__MSP430FR5739__)
		/* Port 0 is not available */
		MAKE_PORT_IE_REN_NO_SEL(P1, Port1, '1')
		MAKE_PORT_IE_REN_NO_SEL(P2, Port2, '2')
		MAKE_PORT_IE_REN_NO_SEL(P3, Port3, '3')
		MAKE_PORT_IE_REN_NO_SEL(P4, Port4, '4')
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

