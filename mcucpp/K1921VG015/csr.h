/// @file
/// @brief Architecture specific CSR's defs and inlines

#ifndef SCR_INFRA_CSR_H
#define SCR_INFRA_CSR_H

#ifndef __ASSEMBLER__

#include "stringify.h"

#define swap_csr(reg, val)                      \
({                                              \
    unsigned long __tmp = (unsigned long)(val); \
    asm volatile ("csrrw %0," _TOSTR(reg) ",%1" \
                  : "=r" (__tmp) : "rK" (__tmp) \
                  : "memory");                  \
    __tmp;                                      \
})

#define read_csr(reg)                           \
({                                              \
    unsigned long __tmp;                        \
    asm volatile ("csrr %0," _TOSTR(reg)        \
                  : "=r" (__tmp) :              \
                  : "memory");                  \
    __tmp;                                      \
})

#define write_csr(reg, val)                     \
({                                              \
    unsigned long __tmp = (unsigned long)(val); \
    asm volatile ("csrw " _TOSTR(reg) ",%0"     \
                  : : "rK" (__tmp)              \
                  : "memory");                  \
})

#define set_csr(reg, val)                       \
({                                              \
    unsigned long __tmp = (unsigned long)(val); \
    asm volatile ("csrrs %0," _TOSTR(reg) ",%1" \
                  : "=r" (__tmp) : "rK" (__tmp) \
                  : "memory");                  \
    __tmp;                                      \
})

#define clear_csr(reg, val)                     \
({                                              \
    unsigned long __tmp = (unsigned long)(val); \
    asm volatile ("csrrc %0," _TOSTR(reg) ",%1" \
                  : "=r" (__tmp) : "rK" (__tmp) \
                  : "memory");                  \
    __tmp;                                      \
})

#define rdtime() read_csr(time)
#define rdcycle() read_csr(cycle)
#define rdinstret() read_csr(instret)

#else // !__ASSEMBLER__

// assembler macros

.altmacro

.macro csr_write_imm csr_reg, val
    .if \val == 0
    csrw \csr_reg, zero
    .elseif (\val / 32) == 0
    csrwi \csr_reg, \val
    .else
    li   t0, \val
    csrw \csr_reg, t0
    .endif
.endm

#endif // !__ASSEMBLER__
#endif // SCR_INFRA_CSR_H
