
#ifndef SCR1_CSR_ENCODING_H
#define SCR1_CSR_ENCODING_H

// Bit mask and register value list is based on 
// SCR1 External Architecture Specification
// (3.2.4. Machine Mode Standard CSRs)

#define MVENDORID_VALUE             0x0
#define MARCHID_VALUE               0x8
#define MIMPID_VALUE                0x19083000

// MSTATUS masks
//
#define MSTATUS_MIE                 (0x1 << 3)
#define MSTATUS_MPIE                (0x1 << 7)
#define MSTATUS_MPP                 (0x3 << 11)

// MISA
// 
#define MISA_RVC                    (0x1 << 2)
#define MISA_RVE                    (0x1 << 4)
#define MISA_RVI                    (0x1 << 8)
#define MISA_RVM                    (0x1 << 12)
#define MISA_RVX                    (0x1 << 23)
#define MISA_MXL                    (0x3 << 30)

// MIE
//
#define MIE_MSIE                    (0x1 << 3)
#define MIE_MTIE                    (0x1 << 7)
#define MIE_MEIE                    (0x1 << 11)

// MTVEC
//
#define MTVEC_MODE                  (0x3 << 0)
#define MTVEC_MODE_DIRECT           (0x0 << 0)
#define MTVEC_MODE_VECTORED         (0x1 << 0)
#define MTVEC_BASE                  (0xFFFFFFFF & 0xFFFFFFC0)

// MSCRATCH
// MEPC

// MCAUSE
//
#define MCAUSE_EC                   (0xF << 0)
#define MCAUSE_INT                  (0x1 << 31)
#define MCAUSE_INSTRUCTION_ADDRESS_MISALIGNED   0
#define MCAUSE_INSTRUCTION_ACCESS_FAULT         1
#define MCAUSE_ILLEGAL_INSTRUCTION              2
#define MCAUSE_BREAKPOINT                       3
#define MCAUSE_LOAD_ADDRESS_MISSALIGNED         4
#define MCAUSE_LOAD_ACCESS_FAULT                5
#define MCAUSE_STORE_ADDRESS_MISALIGNED         6
#define MCAUSE_STORE_ACCESS_FAULT               7
//
#define MCAUSE_ECALL_FROM_M_MODE                11
//
#define MCAUSE_MACHINE_SOFTWARE_INTERRUPT       0x80000003
#define MCAUSE_MACHINE_TIMER_INTERRUPT          0x80000007
#define MCAUSE_MACHINE_EXTERNAL_INTERRUPT       0x8000000B

// MTVAL

// MIP
//
#define MIP_MSIP                    (0x1 << 3)
#define MIP_MTIP                    (0x1 << 7)
#define MIP_MEIP                    (0x1 << 11)

// MCYCLE
// MCYCLEH
// MINSTRET
// MINSTRETH

// MCOUNTEN
#define MCOUNTEN_CY                 (1 << 0)
#define MCOUNTEN_IR                 (1 << 2)

#endif
