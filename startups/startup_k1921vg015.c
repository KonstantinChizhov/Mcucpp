### NIIET K1921VG015 startup code
###
### @copyright (C) NIIET 2022. All rights reserved.

#include "arch.h"
#include "memasm.h"

### #########################
### startup code

    .globl _start, main, exit, abort, trap_handler, trap_entry, _hart_halt, plf_init, app_init
    .weak exit, abort, trap_entry, trap_handler, app_init

    .option norvc

    .section ".startup.entry","ax",@progbits

    ## Entry point
_start:
    ## reset mstatus
    csrw  mstatus, zero

#ifdef PLF_INIT_REGS
    init_regs_const 1, 15, 0
#ifndef __riscv_32e
    init_regs_const 16, 31, 0
#endif // !__riscv_32e
#endif // PLF_INIT_REGS

    ## setup gp
    load_addrword_abs gp, __global_pointer$
    ## setup MTVEC
    load_addrword_abs t0, trap_entry
    csrw  mtvec, t0
    ## setup MIE, MIP


    csrw  mie, zero
    csrw  mip, zero

#if PLF_CACHE_CFG
    ## prepare disabling of caches: flush init sections
    load_addrword_pcrel t0, __TEXT_INIT_START__
    load_const_int32 t1, __TEXT_INIT_SIZE__
    cache_flush t0, t1
#endif // PLF_CACHE_CFG

    ## init FPU (if supported)
    csrr  a0, misa
    andi  a0, a0, (1 << ('F' - 'A')) | (1 << ('D' - 'A'))
    beqz  a0, 1f
    li    a0, (1 << 13) // mstatus.FS = 1 (initial)
    csrs  mstatus, a0
    csrw  fcsr, zero
1:

#if PLF_SMP_SUPPORT
#define slave_hart_start secondary_spin_wait
#else
#define slave_hart_start _hart_halt
#endif // PLF_SMP_SUPPORT

    ## park/prep SMP slaves
    csrr  a0, mhartid
    ## TODO: alt harts enumeration (slot#, lottery, etc)

#if PLF_SMP_MASTER_HARTID
#if PLF_SMP_MASTER_HARTID > 0
    li    t0, PLF_SMP_MASTER_HARTID
    bne   a0, t0, slave_hart_start
#endif // PLF_SMP_MASTER_HARTID > 0
#else // PLF_SMP_MASTER_HARTID
    bnez  a0, slave_hart_start
#endif // PLF_SMP_MASTER_HARTID

    ## init HART#0 sp, tp
    load_addrword_abs sp, __TLS0_BASE__
    mv    tp, sp
    csrw  mscratch, sp
#if PLF_TRAP_STACK
    // reserve trap stack space
    li    t0, PLF_TRAP_STACK
    sub   sp, sp, t0
#endif // PLF_TRAP_STACK
    ## platform init
    load_addrword t0, plf_init
    jalr  t0
    ## jal  plf_init
    ## application-specific initialization hook
    load_addrword t0, app_init
    jalr  t0
    ## jal  app_init
    ## main() args
    li    a0, 0
    li    a1, 0
#if PLF_SMP_SUPPORT
    ## kick slaves
    .globl plf_smp_sync_var
    li    t0, 1
    load_addrword t1, plf_smp_sync_var
    sw    t0, (t1)
#if PLF_SMP_NON_COHERENT
    clflush t1
#endif // PLF_SMP_NON_COHERENT
#endif // PLF_SMP_SUPPORT
    ## start main
    load_addrword t0, main
    load_addrword ra, exit
    jr    t0

trap_handler:
## halt_loop:
    ## halt hart
_hart_halt:
#if PLF_HTIF_BASE
#if PLF_SMP_SUPPORT
    ## do shutdown from master hart only
    csrr  a0, mhartid
    ## TODO: alt harts enumeration (slot#, lottery, etc)
#if PLF_SMP_MASTER_HARTID
#if PLF_SMP_MASTER_HARTID > 0
    li    t0, PLF_SMP_MASTER_HARTID
    bne   a0, t0, 2f
#endif // PLF_SMP_MASTER_HARTID > 0
#else // PLF_SMP_MASTER_HARTID
    bnez  a0, 2f
#endif // PLF_SMP_MASTER_HARTID
#endif // PLF_SMP_SUPPORT
    load_addrword_abs t0, PLF_HTIF_BASE
    li    t1, 1
1:
#if __riscv_xlen == 32
    ## fromhost <= 0 (ACK)
    sw    zero, 8(t0)
    sw    zero, 12(t0)
    ## tohost <= 1 (do shutdown)
    sw    t1,   0(t0)
    sw    zero, 4(t0)
#else // __riscv_xlen == 32
    ## fromhost <= 0 (ACK)
    sd    zero, 8(t0)
    ## tohost <= 1 (do shutdown)
    sd    t1,   0(t0)
#endif // __riscv_xlen == 32
    j     1b
#if PLF_SMP_SUPPORT
2:
    ## disable interrupts
    csrci mstatus, 0xf
    csrw  mie, zero
1:  wfi
    j     1b
#endif // PLF_SMP_SUPPORT
#else // PLF_HTIF_BASE
    ## disable interrupts
    csrci mstatus, 0xf
    csrw  mie, zero
1:  wfi
    j     1b
#endif // PLF_HTIF_BASE

exit:
    nop
abort:
    j     _hart_halt

#if PLF_SMP_SUPPORT
    .globl hart_start_table, plf_smp_slave_init, PLF_SMP_SLAVE_ENTRY
secondary_spin_wait:
    ## park unsupported harts
    li    a1, PLF_SMP_HART_NUM
    bgeu  a0, a1, _hart_halt
    load_addrword tp, hart_start_table
#if __riscv_xlen == 32
    sll   a1, a0, 2
#else // __riscv_xlen == 32
    sll   a1, a0, 3
#endif // __riscv_xlen == 32
    add   tp, tp, a1
    ## wait master init
1:
#if PLF_SMP_NON_COHERENT
    clinval tp
#endif // PLF_SMP_NON_COHERENT
    load_reg_offs sp, 0, tp
    ## wait for valid address value: addr != 0 and (addr & 3) == 0
    beqz  sp, 1b
    andi  a1, sp, 3
    bnez  a1, 1b
    ## store invalid addr value to prevent infinity loops
    li    a1, -1
    save_reg_offs a1, 0, tp
#if PLF_SMP_NON_COHERENT
    clflush tp
#endif // PLF_SMP_NON_COHERENT
    mv    tp, sp
#if PLF_TRAP_STACK
    // reserve trap stack space
    li    t0, PLF_TRAP_STACK
    sub   sp, sp, t0
#endif // PLF_TRAP_STACK
    jal   plf_smp_slave_init
    ## start main
    li    a0, 0
    li    a1, 0
    load_addrword t0, PLF_SMP_SLAVE_ENTRY
    load_addrword ra, exit
    jr    t0
#endif // PLF_SMP_SUPPORT

    ## stubs
app_init:
    ret

#if PLF_SAVE_RESTORE_REGS331_SUB
### trap helpers: save/restore regs x3-x31
    .section ".text.crt.plf_save_regs331_sub","ax",@progbits
    .align 4
    .type plf_save_regs331_sub, @function
plf_save_regs331_sub:
    save_regs 3, 15, sp      // save x3 - x15
#ifndef __riscv_32e
    save_regs 16, 31, sp     // save x16 - x31
#endif //  __riscv_32e
    ret
    .size plf_save_regs331_sub, .-plf_save_regs331_sub

    .section ".text.crt.plf_restore_regs331_sub","ax",@progbits
    .align 4
    .type plf_restore_regs331_sub, @function
plf_restore_regs331_sub:
    load_regs 3, 15, sp      // restore x3 - x15
#ifndef __riscv_32e
    load_regs 16, 31, sp     // restore x16 - x31
#endif //  __riscv_32e
    ret
    .size plf_restore_regs331_sub, .-plf_restore_regs331_sub
#endif // PLF_SAVE_RESTORE_REGS331_SUB

### #########################
### trap handler
    .section ".text.crt.trap_entry","ax",@progbits
    .align 6
    .type trap_entry, @function
trap_entry:
    ## save context
    context_save
    ## save mstatus priv stack
    csrr s0, mstatus
    ## load trap handler args
    csrr a0, mcause
    csrr a1, mepc
    mv   a2, sp

    ## setup gp
    load_addrword_abs gp, __global_pointer$
    ## call trap handler
    load_addrword t0, trap_handler
    jalr t0

    ## restore mstatus priv stack
    csrw mstatus, s0
    ## restore context
    context_restore
    mret
    .size trap_entry, .-trap_entry
