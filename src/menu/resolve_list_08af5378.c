/* Resolves up to four ids from the constant list (stopping at the first end marker) and passes them on. */
extern int func_08AF5248(unsigned short a, int id);
extern void func_08AF5444(void *a, void *b, int *list, void *c);

extern unsigned short D_08B00CA2[];

void func_08AF5378(void *a, void *b, short key, void *c)
{
    int out[4];
    int *dst;
    int i = 0;
    unsigned short *src = D_08B00CA2;
    unsigned short end = 0xFFFF;
    unsigned short k = key;
    dst = out;
    do {
        if (*src == end) {
            break;
        }
        *dst = func_08AF5248(k, *src);
        i++;
        src += 2;
        dst++;
    } while (i < 4);
    func_08AF5444(a, b, out, c);
}
