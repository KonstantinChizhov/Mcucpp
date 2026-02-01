#ifndef __SCR1__SPECIFIC
#define __SCR1__SPECIFIC

#define mcounten        0x7E0

// Memory-mapped registers
#define mtime_base      0x00490000
#define mtime_ctrl_offs 0x00
#define mtime_div_offs  0x04
#define mtime_offs      0x08
#define mtimeh_offs     0x0C
#define mtimecmp_offs   0x10
#define mtimecmph_offs  0x14
#define mtime_ctrl      mtime_base+mtime_ctrl_offs
#define mtime_div       mtime_base+mtime_div_offs
#define mtime           mtime_base+mtime_offs
#define mtimeh          mtime_base+mtimeh_offs
#define mtimecmp        mtime_base+mtimecmp_offs
#define mtimecmph       mtime_base+mtimecmph_offs

#define SCR1_MTIME_CTRL_EN          0
#define SCR1_MTIME_CTRL_CLKSRC      1

#define SCR1_MTIME_CTRL_WR_MASK     0x3
#define SCR1_MTIME_DIV_WR_MASK      0x3FF

#endif // _SCR1__SPECIFIC
