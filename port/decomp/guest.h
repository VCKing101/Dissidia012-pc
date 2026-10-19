/* What decompiled code sees of the game: guest memory and guest calls.
 *
 * The functions in port/decomp/ are the game's own functions, rewritten in C
 * from the machine code of ULUS10566 v1.00 and checked against the recompiled
 * originals (port/host/d012_decomp_harness.cpp). They do not match the
 * original bytes; they behave the same: the same memory written, the same
 * calls made in the same order with the same arguments, the same results.
 *
 * A Guest is one machine to run on. The port has two: the game itself (the
 * live runtime, port/host/d012_decomp_harness.cpp) and an isolated reference
 * runtime the harness uses to compare a decompiled function with the
 * original from the same memory and registers. Decompiled code cannot tell
 * them apart.
 *
 * Guest pointers stay guest addresses (gaddr). Structures in guest memory are
 * read through host pointers from g_ptr(): guest RAM is one block, little
 * endian like the host, with the guest's natural alignment.
 *
 * MIT licence; see port/README.md. */
#ifndef D012_DECOMP_GUEST_H
#define D012_DECOMP_GUEST_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t gaddr;

/* Arguments and results of one guest call, in the Allegrex ABI: a[0..3] are
 * $a0-$a3 and a[4..7] are $t0-$t3 (the eight integer argument registers the
 * game's code uses), f[0..3] are $f12-$f15. */
typedef struct GuestRegs {
    uint32_t a[8];
    float f[4];
    uint32_t v0, v1;
    float f0;
} GuestRegs;

typedef struct Guest Guest;
struct Guest {
    uint8_t *ram;      /* host address of guest 0x08000000 */
    uint32_t ram_size;
    /* The guest stack pointer: the caller's $sp at entry. A decompiled
     * function that hands its callees pointers into its own stack frame
     * reserves the frame with g_push(), as the original does. */
    gaddr sp;
    /* Runs the guest function at `fn` to its return, with $sp = g->sp. */
    void (*call)(Guest *g, gaddr fn, GuestRegs *regs);
    /* An address outside guest RAM, or another fault: does not return. */
    void (*fault)(Guest *g, const char *what, gaddr address);
    /* Set by g_tail(): when the decompiled function returns, the caller of
     * the decompiled function continues at `tail_fn` with these registers,
     * as if the original had jumped there with its own return address. */
    int tail_pending;
    gaddr tail_fn;
    GuestRegs tail_regs;
    void *impl;
};

#define GUEST_RAM_BASE 0x08000000u

static inline void *g_ptr(Guest *g, gaddr address, uint32_t length) {
    const uint32_t offset = (address & 0x1FFFFFFFu) - GUEST_RAM_BASE;
    if (offset >= g->ram_size || g->ram_size - offset < length) g->fault(g, "access outside guest RAM", address);
    return g->ram + offset;
}
#define G_STRUCT(g, type, address) ((type *)g_ptr((g), (address), (uint32_t)sizeof(type)))

static inline uint32_t g_ld32(Guest *g, gaddr a) { uint32_t v; memcpy(&v, g_ptr(g, a, 4), 4); return v; }
static inline int32_t g_ld32s(Guest *g, gaddr a) { return (int32_t)g_ld32(g, a); }
static inline int16_t g_ld16s(Guest *g, gaddr a) { int16_t v; memcpy(&v, g_ptr(g, a, 2), 2); return v; }
static inline uint8_t g_ld8(Guest *g, gaddr a) { return *(uint8_t *)g_ptr(g, a, 1); }
static inline int8_t g_ld8s(Guest *g, gaddr a) { return (int8_t)g_ld8(g, a); }
static inline void g_st32(Guest *g, gaddr a, uint32_t v) { memcpy(g_ptr(g, a, 4), &v, 4); }
static inline void g_st16(Guest *g, gaddr a, uint16_t v) { memcpy(g_ptr(g, a, 2), &v, 2); }
static inline void g_st8(Guest *g, gaddr a, uint8_t v) { *(uint8_t *)g_ptr(g, a, 1) = v; }
static inline void g_stf(Guest *g, gaddr a, float v) { memcpy(g_ptr(g, a, 4), &v, 4); }

/* A stack frame of `size` bytes for buffers the callees fill. */
static inline gaddr g_push(Guest *g, uint32_t size) { g->sp -= size; return g->sp; }
static inline void g_pop(Guest *g, uint32_t size) { g->sp += size; }

/* Calls with up to eight integer arguments; the result is $v0. */
static inline uint32_t g_call8(Guest *g, gaddr fn, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3,
                               uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3) {
    GuestRegs r;
    memset(&r, 0, sizeof r);
    r.a[0] = a0; r.a[1] = a1; r.a[2] = a2; r.a[3] = a3;
    r.a[4] = t0; r.a[5] = t1; r.a[6] = t2; r.a[7] = t3;
    g->call(g, fn, &r);
    return r.v0;
}
static inline uint32_t g_call4(Guest *g, gaddr fn, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {
    return g_call8(g, fn, a0, a1, a2, a3, 0, 0, 0, 0);
}
static inline uint32_t g_call5(Guest *g, gaddr fn, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3,
                               uint32_t t0) {
    return g_call8(g, fn, a0, a1, a2, a3, t0, 0, 0, 0);
}
static inline uint32_t g_call3(Guest *g, gaddr fn, uint32_t a0, uint32_t a1, uint32_t a2) {
    return g_call4(g, fn, a0, a1, a2, 0);
}
static inline uint32_t g_call2(Guest *g, gaddr fn, uint32_t a0, uint32_t a1) { return g_call4(g, fn, a0, a1, 0, 0); }
static inline uint32_t g_call1(Guest *g, gaddr fn, uint32_t a0) { return g_call4(g, fn, a0, 0, 0, 0); }
/* One float argument in $f12 and four integers. */
static inline uint32_t g_call_f4(Guest *g, gaddr fn, float f12, uint32_t a0, uint32_t a1, uint32_t a2,
                                 uint32_t a3) {
    GuestRegs r;
    memset(&r, 0, sizeof r);
    r.f[0] = f12;
    r.a[0] = a0; r.a[1] = a1; r.a[2] = a2; r.a[3] = a3;
    g->call(g, fn, &r);
    return r.v0;
}

/* Ends the decompiled function with a jump to `fn` (a tail call). */
static inline void g_tail(Guest *g, gaddr fn, const GuestRegs *regs) {
    g->tail_pending = 1;
    g->tail_fn = fn;
    g->tail_regs = *regs;
}

/* A C++ virtual call as the game's compiler makes it: the vtable pointer at
 * `vtable_offset` in the object, then per slot an s16 this-adjustment and the
 * function. A 64-bit argument goes in $a2:$a3. */
static inline uint32_t g_vcall_u64(Guest *g, gaddr object, uint32_t vtable_offset, uint32_t slot, uint32_t lo,
                                   uint32_t hi) {
    const gaddr vtable = g_ld32(g, object + vtable_offset);
    const int16_t adjust = g_ld16s(g, vtable + slot);
    const gaddr fn = g_ld32(g, vtable + slot + 4u);
    /* $a1 still holds the function address, as at the original call sites. */
    return g_call4(g, fn, object + (uint32_t)(int32_t)adjust, fn, lo, hi);
}

/* The Allegrex remainder as PortableKit's recompiler gives it: the dividend
 * when dividing by zero. */
static inline int32_t g_rem(int32_t dividend, int32_t divisor) {
    if (divisor == 0) return dividend;
    if (dividend == INT32_MIN && divisor == -1) return 0;
    return dividend % divisor;
}

#ifdef __cplusplus
}
#endif

#endif
