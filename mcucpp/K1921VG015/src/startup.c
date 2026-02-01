
#include <stdint.h>
#include <stddef.h>
#include <csr.h>
#include <string.h>

void trap_handler(void);
void trap_entry(void);
int main(int argc, char **argv);

extern unsigned char __bss_start, __bss_end;
extern unsigned char __data_target_start, __data_target_end, __data_source_start;
extern unsigned char __sdata_target_start, __sdata_target_end, __sdata_source_start;
extern unsigned char __STACK_END__;

extern unsigned long __ctors_start__;
extern unsigned long __ctors_end__;
extern unsigned long __fini_array_start;
extern unsigned long __fini_array_end;

void *__dso_handle = (void *)&__dso_handle;

__attribute__((section(".startup.entry"))) __attribute__((noreturn)) __attribute__((naked)) void _start()
{

    write_csr(mstatus, 0);
    asm volatile(
        ".option push		\n"
        ".option norelax	\n"
        "la gp, __global_pointer$ \n"
        ".option pop	\n");
    write_csr(mtvec, trap_entry);
    write_csr(mie, 0);
    write_csr(mip, 0);
    set_csr(mstatus, (0b01 << 13)); // FS=0b01, FPU status: initial
    write_csr(fcsr, 0);

    asm volatile(
        "la sp, __STACK_END__ \n"
        "mv tp, sp            \n"
        "csrw mscratch, sp    \n");

    memset(&__bss_start, 0, (size_t)(&__bss_end - &__bss_start));

    memcpy((void *)&__data_target_start,
           (const void *)&__data_source_start,
           (&__data_target_end - &__data_target_start));

    memcpy((void *)&__sdata_target_start,
           (const void *)&__sdata_source_start,
           (&__sdata_target_end - &__sdata_target_start));

    // call global object ctors

    typedef void (*voidFunc)();
    for (unsigned long *pDest = &__ctors_start__; pDest < &__ctors_end__; pDest++)
    {
        ((voidFunc)(*pDest))();
    }

    main(0, (void *)0);

    for (unsigned long *pDest = (&__fini_array_end - 1); pDest >= &__fini_array_start; pDest--)
    {
        ((voidFunc)(*pDest))();
    }

    while (1)
    {
    }
}

__attribute__((weak)) __attribute__((interrupt)) void trap_entry(void)
{
    trap_handler();
}