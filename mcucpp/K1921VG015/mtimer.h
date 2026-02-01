#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define RISCV_MTIMECMP_ADDR (0x2000000 + 0x4000)
#define RISCV_MTIME_ADDR (0x2000000 + 0xBFF8)


/** Read the raw time of the system timer in system timer clocks
 */

inline __attribute__((always_inline))
uint64_t
mtimer_get_raw_time(void)
{
    volatile uint32_t *const mtimel = (volatile uint32_t *)(RISCV_MTIME_ADDR);
    volatile uint32_t *const mtimeh = (volatile uint32_t *)(RISCV_MTIME_ADDR + 4);

    uint32_t mtimeh_val;
    uint32_t mtimel_val;
    do
    {
        // There is a small risk the mtimeh will tick over after reading mtimel
        mtimeh_val = *mtimeh;
        mtimel_val = *mtimel;
        // Poll mtimeh to ensure it's consistent after reading mtimel
        // The frequency of mtimeh ticking over is low
    } while (mtimeh_val != *mtimeh);
    return (uint64_t)((((uint64_t)mtimeh_val) << 32) | mtimel_val);
}


/** Set the raw time compare point in system timer clocks.
 * @param clock_offset Time relative to current mtime when
 * @note The time range of the 64 bit timer is large enough not to consider a wrap around of mtime.
 * An interrupt will be generated at mtime + clock_offset.
 * See http://five-embeddev.com/riscv-isa-manual/latest/machine.html#machine-timer-registers-mtime-and-mtimecmp
 */

inline void mtimer_set_raw_time_cmp(uint64_t clock_offset)
{
    // First of all set
    uint64_t new_mtimecmp = mtimer_get_raw_time() + clock_offset;

    volatile uint32_t *mtimecmpl = (volatile uint32_t *)(RISCV_MTIMECMP_ADDR);
    volatile uint32_t *mtimecmph = (volatile uint32_t *)(RISCV_MTIMECMP_ADDR + 4);

    // AS we are doing 32 bit writes, an intermediate mtimecmp value may cause spurious interrupts.
    // Prevent that by first setting the dummy MSB to an unacheivable value
    *mtimecmpl = 0xFFFFFFFF; // cppcheck-suppress redundantAssignment
    // Set the correct MSB
    *mtimecmph = (uint32_t)(new_mtimecmp >> 32); // cppcheck-suppress redundantAssignment
    // set the LSB
    *mtimecmpl = (uint32_t)(new_mtimecmp & 0x0FFFFFFFFUL);
}


#endif // #ifdef TIMER_H
