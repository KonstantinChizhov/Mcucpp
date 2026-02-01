/*
 *
 * PLIC source handler file bm310s6 core
 *
 * created by DVS
 *
*/

#include "csr.h"
#include "plic.h"

// pointers to handler functions for machine mode
irqfunc* mach_plic_handler[32] __attribute__((section(".data")));
// pointers to handler functions for supervisor mode
irqfunc* supervisor_plic_handler[32] __attribute__((section(".data")));

typedef struct {
	__IO uint32_t PRI[32];
	uint32_t Reserved1[992];
	__O uint32_t IPM0;
	uint32_t Reserved2[1023];
	__IO uint32_t MIEM0;
	uint32_t Reserved3[31];
	__IO uint32_t UIEM0;
	uint32_t Reserved4[2015];
	__O uint32_t NINT;
	__O uint32_t NPRI;
	uint32_t Reserved5[520190];
	__IO uint32_t MTHR;
	__IO uint32_t MICC;
	uint32_t Reserved6[1022];
	__IO uint32_t UTHR;
	__IO uint32_t UICC;

} PLIC_TypeDef ;

#define PLIC_BASE        (0x0C000000UL)
#define PLIC             ((PLIC_TypeDef  *) PLIC_BASE)

// private functions
uint32_t PLIC_ClaimIrq      (uint8_t target);
void     PLIC_ClaimComplete (uint8_t target, uint32_t isrnum);

// defines
#define NULL_IRQ 0

/*
 * Set PLIC handler function for isr num
 */

void PLIC_SetIrqHandler (uint8_t target, uint32_t isr_num, irqfunc* func)
{
    if(target == Plic_Mach_Target) {
        mach_plic_handler[isr_num] = func;
    } else {
        supervisor_plic_handler[isr_num] = func;
    }
}
/*
 * Set PLIC irq priority
 */

void PLIC_SetPriority (uint32_t isr_num, uint8_t pri)
{
    PLIC->PRI[isr_num] = pri;
}

/*
 * Set PLIC irq mode
 */

/*void PLIC_SetMode (uint32_t isr_num, Plic_IrqMode_TypeDef mode)
{
    PLIC->SRC_MODE[isr_num] = (uint32_t)(mode);
}*/

/*
 * Enable PLIC irq
 */

void PLIC_IntEnable (uint8_t target, uint32_t isr_num)
{
	uint32_t value;

	//read - modify - write operation

    if(target == Plic_Mach_Target) {
    	value  = PLIC->MIEM0;
    	value |= (1<<isr_num);
    	PLIC->MIEM0 = value;
    } else {
    	value = PLIC->UIEM0;
    	value |= (1<<isr_num);
    	PLIC->UIEM0 = value;
    }
}

/*
 * Disable PLIC irq
 */

void PLIC_IntDisable (uint8_t target, uint32_t isr_num)
{
	uint32_t value;

	//read - modify - write operation

	if(target == Plic_Mach_Target) {
		value  = PLIC->MIEM0;
		value &= ~(1<<isr_num);
		PLIC->MIEM0 = value;
	} else {
		value  = PLIC->UIEM0;
		value &= ~(1<<isr_num);
		PLIC->UIEM0 = value;
	}
}

/*
 * Get current isr number
 */

uint32_t PLIC_ClaimIrq (uint8_t target)
{
	if(target == Plic_Mach_Target) {
		return (uint32_t)(PLIC->MICC);
	} else {
		return (uint32_t)(PLIC->UICC);
	}
}

/*
 * Set current isr as completed
 */

void PLIC_ClaimComplete (uint8_t target, uint32_t isrnum)
{
	if(target == Plic_Mach_Target) {
		PLIC->MICC = isrnum;
	} else {
		PLIC->UICC = isrnum;
	}
}

/*
 * Set threshold for irqs
 */

void PLIC_SetThreshold(uint8_t target, uint32_t value) {
	if(target == Plic_Mach_Target) {
		PLIC->MTHR = value;
	} else {
		PLIC->UTHR = value;
	}
}


void PLIC_MachHandler(void) {

	// handle interrupt
	uint32_t isr_num = PLIC_ClaimIrq(Plic_Mach_Target);
	// check if handler exist
	if(mach_plic_handler[isr_num] != NULL_IRQ) {
		// call isr handler
		mach_plic_handler[isr_num]();
		// set isr completes
		PLIC_ClaimComplete(Plic_Mach_Target, isr_num);
	}
}

// machine irq handler
#define MCAUSE_INTERRUPT_FLAG           (1<<31)
#define MCAUSE_EXCEPT_MASK              0xF
#define MCAUSE_EXCEPT_INSTRADDRMISALGN  0x0
#define MCAUSE_EXCEPT_INSTRACCSFAULT    0x1
#define MCAUSE_EXCEPT_INSTRILLEGAL      0x2
#define MCAUSE_EXCEPT_BREAKPNT          0x3
#define MCAUSE_EXCEPT_LOADADDRMISALGN   0x4
#define MCAUSE_EXCEPT_LOADACCSFAULT     0x5
#define MCAUSE_EXCEPT_STAMOADDRMISALGN  0x6
#define MCAUSE_EXCEPT_STAMOACCSFAULT    0x7
#define MCAUSE_EXCEPT_ECALLFRM_M_MODE   0xB

void trap_handler (void)
{
	uint32_t mcause_val = read_csr(mcause);

	if((mcause_val & MCAUSE_INTERRUPT_FLAG) == 0) {
		// handle exception
		switch (mcause_val & MCAUSE_EXCEPT_MASK)
		{
			case MCAUSE_EXCEPT_INSTRADDRMISALGN:
				break;
			case MCAUSE_EXCEPT_INSTRACCSFAULT:
				break;
			case MCAUSE_EXCEPT_INSTRILLEGAL:
				break;
			case MCAUSE_EXCEPT_BREAKPNT:
				break;
			case MCAUSE_EXCEPT_LOADADDRMISALGN :
				break;
			case MCAUSE_EXCEPT_LOADACCSFAULT:
				break;
			case MCAUSE_EXCEPT_STAMOADDRMISALGN:
				break;
			case MCAUSE_EXCEPT_STAMOACCSFAULT:
				break;
			case MCAUSE_EXCEPT_ECALLFRM_M_MODE:
				break;

			default: // MCAUSE_EXCEPT UNKNOWN

				break;
		}

		while(1) {}; //TRAP
	} else {
		// handle interrupt
		PLIC_MachHandler();
	}
}


