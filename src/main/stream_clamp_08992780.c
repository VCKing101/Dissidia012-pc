/* Clamps a read length to the stream limits and reports whether the cached position changed. */
typedef struct St { char pad0[4]; int h; int base; int size; char pad1[0x44 - 0x10]; int pos; } St;
extern int func_089C5CC0(int);
extern void func_089C5D08(int, int *);
int func_08992780(St *st, unsigned *off, unsigned *len, int *pos)
{
    int res;
    if (func_089C5CC0(st->h) < 0) {
        int tmp[1];
        tmp[0] = 0;
        func_089C5D08(st->h, tmp);
        *len = 0;
        return 0;
    }
    res = 0;
    if (*len > 0x20000) *len = 0x20000;
    if (st->base + st->size < *off + *len) *len = (st->base + st->size) - *off;
    if (st->pos != *pos) {
        res = 1;
        st->pos = *pos;
    }
    return res;
}
