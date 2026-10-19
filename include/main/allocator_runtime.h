#ifndef MAIN_ALLOCATOR_RUNTIME_H
#define MAIN_ALLOCATOR_RUNTIME_H
#ifdef __cplusplus
extern "C" {
#endif
/* Native entry uses signed alignment, selecting heap +0xC when alignment
 * is nonpositive. The final flags are narrowed to bytes; argument four is
 * overwritten before use. Keep its ABI word explicit. */
extern void *func_0885C6CC(void *heap, unsigned int size, int alignment,
                          unsigned int unused_word, unsigned char first_flag,
                          unsigned char second_flag);
extern void func_0885C538(void *heap, void *buffer, int alignment);
#ifdef __cplusplus
}
#endif
#endif
