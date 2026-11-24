// Looks up a value with func_08ACF234 on the block at +0xD5C and applies it to the selected 198-byte table entry via func_088B7A88.
extern int func_08ACF234(void *);
extern void func_088B7A88(void *, int, int);
typedef struct { char pad0[0x30C]; char *tbl; char pad1[0xD5C - 0x310]; char blk[1]; } Obj08B33B08;
void func_08B33B08(Obj08B33B08 *self, int b)
{
    char *t = self->tbl;
    int idx = *(signed char *)(t + 0x773);
    char *e = t + idx * 198 + 0x10;
    int v = func_08ACF234(self->blk);
    func_088B7A88(e, v, b);
}
