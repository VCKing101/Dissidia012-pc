/* out = clamp01(in * a * (b / 32768)), with a and b two 16-byte vectors found
 * through a table of offsets from the owner's base (next entry pair idx+2, idx). */
#include "vfpu.h"

typedef struct OffsetTable {
    int field_00;
    unsigned int index;
    unsigned short *offsets;
} OffsetTable;

typedef struct BlendSource {
    char pad[0x8C];
    char *base;
    OffsetTable *table;
} BlendSource;

void func_0897CC8C(VfpuVec4 *out, const VfpuVec4 *in, BlendSource *src)
{
    char *base = src->base;
    OffsetTable *t = src->table;
    VfpuVec4 *a = (VfpuVec4 *)(base + t->offsets[t->index + 2] + 0x60);
    VfpuVec4 *b = (VfpuVec4 *)(base + t->offsets[t->index] + 0x60);
    float scale = 1.0f / 32768.0f;
    __asm__ volatile(
        "lv.q     C000, %1\n"
        "lv.q     C010, %2\n"
        "lv.q     C020, %3\n"
        "lv.s     S030, %4\n"
        "vmul.q   C100, C000, C010\n"
        "vscl.q   C110, C020, S030\n"
        "vmul.q   C120, C100, C110\n"
        "vsat0.q  C130, C120\n"
        "sv.q     C130, %0\n"
        : "=m"(*out)
        : "m"(*in), "m"(*a), "m"(*b), "m"(scale));
}
