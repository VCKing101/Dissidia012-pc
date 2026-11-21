typedef struct Saved0884A944 {
    unsigned char unk_0[4];
    void *data;                        /* +0x4 */
    int header;                        /* +0x8 */
} Saved0884A944;

extern Saved0884A944 *D_089D4D78;
extern void *func_088820D8(int size);                           /* heap_alloc (system heap b) */
extern void *func_08935524(void *dst, const void *src, int size); /* memcpy */

/* Keep a copy of a block: its word at +4 goes to the header, the rest (after 8 bytes) is copied. */
void func_0884A944(char *block, int size)
{
    D_089D4D78->header = *(int *)(block + 4);
    D_089D4D78->data = func_088820D8(size);
    func_08935524(D_089D4D78->data, block + 8, size - 8);
}
