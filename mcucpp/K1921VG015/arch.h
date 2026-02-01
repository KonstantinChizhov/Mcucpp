/// @file
/// @brief Architecture specific defs and inlines

#ifndef SCR_INFRA_ARCH_H
#define SCR_INFRA_ARCH_H

#ifdef __ASSEMBLER__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else // __ASSEMBLER__
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif // __ASSEMBLER__

#define _UL(x)		(_AC(x, UL))
#define _ULL(x)		(_AC(x, ULL))


#if __riscv_xlen == 32
#define EXPAND32ADDR(x) (x)
#else
#define EXPAND32ADDR(x) (((((x) / 0x80000000) & 1) * 0xffffffff00000000) + (x))
#endif

//#include "plf.h"
#include "stringify.h"

#ifndef PLF_HAVE_MPU
#define PLF_HAVE_MPU (PLF_CORE_VARIANT_SCR > 1)
#endif // PLF_HAVE_MPU

#ifndef PLF_HAVE_MMU
#define PLF_HAVE_MMU (PLF_CORE_VARIANT_SCR >= 5)
#endif // PLF_HAVE_MMU

#ifndef PLF_CPU_CLK
#define PLF_CPU_CLK PLF_SYS_CLK
#endif

// platform info
#ifndef PLF_CPU_NAME
#if __riscv_xlen == 32
#define PLF_CPU_NAME _TOSTR(PLF_CORE_VARIANT)
#else
#define PLF_CPU_ISA "/RV" _TOSTR(__riscv_xlen)
#define PLF_CPU_NAME _TOSTR(PLF_CORE_VARIANT) PLF_CPU_ISA
#endif
#endif
#ifndef PLF_IMPL_STR
#define PLF_IMPL_STR "custom"
#endif

// platform-specific CSRs
#ifndef SCR_CSR_MPU_BASE
#define SCR_CSR_MPU_BASE 0xbc4
#endif

// cache support
#ifndef PLF_CACHELINE_SIZE
#define PLF_CACHELINE_SIZE 0
#endif

// separate trap stack size (0 if not used)
#ifndef PLF_TRAP_STACK
#define PLF_TRAP_STACK 0
#endif // PLF_TRAP_STACK

#ifndef PLF_ATOMIC_SUPPORTED
#ifdef __riscv_atomic
#define PLF_ATOMIC_SUPPORTED 1
#else // __riscv_atomic
#define PLF_ATOMIC_SUPPORTED 0
#endif // __riscv_atomic
#endif // !PLF_ATOMIC_SUPPORTED

// trap handler smart save/restore
#ifndef PLF_SAVE_RESTORE_REGS331_SUB
#define PLF_SAVE_RESTORE_REGS331_SUB 0
#endif // PLF_SAVE_RESTORE_REGS331_SUB

// trap causes
// interrupts
#define TRAP_CAUSE_INTERRUPT_FLAG (1UL << (__riscv_xlen - 1))
#define TRAP_CAUSE_INT_SSOFT (1)
#define TRAP_CAUSE_INT_MSOFT (3)
#define TRAP_CAUSE_INT_STIME (5)
#define TRAP_CAUSE_INT_MTIME (7)
#define TRAP_CAUSE_INT_SEXT  (9)
#define TRAP_CAUSE_INT_MEXT  (11)
// exceptions
#define TRAP_CAUSE_EXC_FETCH_ALIGN  (0)
#define TRAP_CAUSE_EXC_FETCH_ACCESS (1)
#define TRAP_CAUSE_EXC_ILLEGAL      (2)
#define TRAP_CAUSE_EXC_BREAKPOINT   (3)
#define TRAP_CAUSE_EXC_LOAD_ALIGN   (4)
#define TRAP_CAUSE_EXC_LOAD_ACCESS  (5)
#define TRAP_CAUSE_EXC_STORE_ALIGN  (6)
#define TRAP_CAUSE_EXC_STORE_ACCESS (7)
#define TRAP_CAUSE_EXC_UECALL       (8)
#define TRAP_CAUSE_EXC_SECALL       (9)
#define TRAP_CAUSE_EXC_RESERVED1    (10)
#define TRAP_CAUSE_EXC_MECALL       (11)
#define TRAP_CAUSE_EXC_FETCH_PAGE   (12)
#define TRAP_CAUSE_EXC_LOAD_PAGE    (13)
#define TRAP_CAUSE_EXC_RESERVED2    (14)
#define TRAP_CAUSE_EXC_STORE_PAGE   (15)

// mie/mip bits
#define MIE_MSOFTWARE (1 << TRAP_CAUSE_INT_MSOFT)
#define MIE_MTIMER    (1 << TRAP_CAUSE_INT_MTIME)
#define MIE_MEXTERNAL (1 << TRAP_CAUSE_INT_MEXT)
// mstatus bits
#define MSTATUS_MIE   (1 << 3)

#ifndef __ASSEMBLER__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline unsigned long __attribute__((const)) arch_misa(void)
{
    unsigned long res;
    asm volatile("csrr %0, misa" : "=r"(res));
    return res;
}

static inline unsigned long __attribute__((const)) arch_cpuid(void)
{
    return arch_misa();
}

static inline unsigned long __attribute__((const)) arch_impid(void)
{
    unsigned long res;
    asm volatile("csrr %0, mimpid" : "=r"(res));
    return res;
}

static inline unsigned long arch_vendorid(void)
{
    unsigned long res;
    asm volatile("csrr %0, mvendorid" : "=r"(res) :: "memory");
    return res;
}

static inline unsigned long __attribute__((const)) arch_archid(void)
{
    unsigned long res;
    asm volatile("csrr %0, marchid" : "=r"(res));
    return res;
}

static inline unsigned long __attribute__((const)) arch_hartid(void)
{
    unsigned long res;
    asm volatile("csrr %0, mhartid" : "=r"(res));
    return res;
}

static inline unsigned long arch_mtval(void)
{
    unsigned long res;
    asm volatile("csrr %0, mtval" : "=r"(res) :: "memory");
    return res;
}

static inline unsigned long arch_cycle(void)
{
    unsigned long res;
    asm volatile("csrr %0, cycle" : "=r"(res) :: "memory");
    return res;
}

static inline uint64_t arch_cycle64(void)
{
#if __riscv_xlen == 32
	uint32_t lo, hi, tmp;

	asm volatile (
        "1:"
        "csrr %[hi], cycleh ;"
		"csrr %[lo], cycle  ;"
		"csrr %[tmp],cycleh ;"
		"bne  %[hi], %[tmp], 1b"
        : [hi] "=&r" (hi),
          [lo] "=&r" (lo),
          [tmp] "=&r" (tmp)
        :: "memory");

	return ((uint64_t)hi << 32) | lo;
#else //  __riscv_xlen == 32
	return arch_cycle();
#endif // __riscv_xlen == 32
}

static inline unsigned long arch_instret(void)
{
    unsigned long res;
    asm volatile("csrr %0, instret" : "=r"(res) :: "memory");
    return res;
}

static inline uint64_t arch_instret64(void)
{
#if __riscv_xlen == 32
	uint32_t lo, hi, tmp;

	asm volatile (
        "1:"
        "csrr %[hi], instreth ;"
		"csrr %[lo], instret  ;"
		"csrr %[tmp],instreth ;"
		"bne  %[hi], %[tmp], 1b"
        : [hi] "=&r" (hi),
          [lo] "=&r" (lo),
          [tmp] "=&r" (tmp)
        :: "memory");

	return ((uint64_t)hi << 32) | lo;
#else //  __riscv_xlen == 32
	return arch_instret();
#endif // __riscv_xlen == 32
}

static inline void ifence(void)
{
    asm volatile ("fence.i" ::: "memory");
}

static inline void fence(void)
{
    asm volatile ("fence" ::: "memory");
}

static inline void wfi(void)
{
    asm volatile ("wfi" ::: "memory");
}

void __attribute__((noreturn)) _hart_halt(void);

static inline void cpu_relax(void)
{
    long dummy;
    asm volatile ("div %0, %0, zero" : "=r" (dummy) :: "memory");
}

static inline void sfence_vma(void)
{
    asm volatile ("sfence.vma" ::: "memory");
}

static inline void sfence_vma_addr(uintptr_t addr)
{
    asm volatile ("sfence.vma %0, zero" :: "r" (addr) : "memory");
}

static inline void sfence_vma_asid(unsigned asid)
{
    asm volatile ("sfence.vma zero, %0" :: "r" (asid) : "memory");
}

static inline void sfence_vma_addr_asid(uintptr_t addr, unsigned asid)
{
    asm volatile ("sfence.vma %0, %1" :: "r" (addr), "r" (asid) : "memory");
}

static inline void flush_tlb(void)
{
    sfence_vma();
}

static inline uint32_t __attribute__((const)) get_build_id(void)
{
#if PLF_BLD_ID_ADDR
    return *(volatile uint32_t*)PLF_BLD_ID_ADDR;
#else
    return 0;
#endif
}

static inline uint32_t __attribute__((const)) get_system_id(void)
{
#ifdef PLF_SYS_ID_ADDR
    return *(volatile uint32_t*)PLF_SYS_ID_ADDR;
#else
    return 0;
#endif
}

static inline int supports_extension(char ext)
{
    return (arch_misa() & (1UL << (ext - 'A'))) != 0;
}

// generic platform initialization function
void plf_init_generic(void);

#ifdef __cplusplus
}
#endif
#endif // __ASSEMBLER__
#endif // SCR_INFRA_ARCH_H
