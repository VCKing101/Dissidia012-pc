/* SDK vector library functions in main's GCC-built range, written the way the
 * library's own source is shaped: GCC inline asm with explicit VFPU registers,
 * "m" operands and a noreorder block. Compile with Sony psp-gcc 1.3.1 -O2 -G0. */
typedef struct { float x, y, z, w; } ScePspFVector4;

#define ASM_BEGIN ".set push\n .set noreorder\n"
#define ASM_END   ".set pop\n"
#define V3OP(name, op)                                                              \
ScePspFVector4 *name(ScePspFVector4 *pv0, const ScePspFVector4 *pv1, const ScePspFVector4 *pv2) \
{                                                                                   \
    __asm__(ASM_BEGIN "lv.q C000, %1\n lv.q C010, %2\n " op " C000, C000, C010\n sv.q C000, %0\n" ASM_END \
            : "=m"(*pv0) : "m"(*pv1), "m"(*pv2));                                    \
    return pv0;                                                                     \
}
#define V4OP(name, op)                                                              \
ScePspFVector4 *name(ScePspFVector4 *pv0, const ScePspFVector4 *pv1, const ScePspFVector4 *pv2) \
{                                                                                   \
    __asm__(ASM_BEGIN "lv.q C010, %1\n lv.q C020, %2\n " op " C000, C010, C020\n sv.q C000, %0\n" ASM_END \
            : "=m"(*pv0) : "m"(*pv1), "m"(*pv2));                                    \
    return pv0;                                                                     \
}
#define VSCL(name, op)                                                              \
ScePspFVector4 *name(ScePspFVector4 *pv0, const ScePspFVector4 *pv1, float t)       \
{                                                                                   \
    __asm__(ASM_BEGIN "mfc1 $8, %2\n mtv $8, S010\n lv.q C000, %1\n " op " C000, C000, S010\n sv.q C000, %0\n" ASM_END \
            : "=m"(*pv0) : "m"(*pv1), "f"(t) : "$8");                                \
    return pv0;                                                                     \
}
#define F1(name, op)                                                                \
float name(float x)                                                                 \
{                                                                                   \
    float f;                                                                        \
    __asm__(ASM_BEGIN "lv.s S000, %1\n " op " S000, S000\n sv.s S000, %0\n" ASM_END \
            : "=m"(f) : "m"(x));                                                     \
    return f;                                                                       \
}

F1(func_089A4704, "vlog2.s")
