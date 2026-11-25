typedef struct V { int a; int b; } V;
typedef struct Wk { char pad[0xA690]; char *b; } Wk;
extern void func_088B97E8(V *, int);
extern void func_08AF6428(void *, V *, int, int);
extern int func_08AF6BBC(int, int);
extern void *D_08B01CF8;
extern void func_08AE70EC(Wk *self, int row, int n)
{
    V v;
    V tmp;
    int first;
    int i;
    unsigned short *ent;
    ent = (unsigned short *)(self->b + row * 0x15F8 + *(unsigned short *)(self->b + row * 0x57E + n * 2 + 0x7EC0) * 8 + 0x26B8);
    func_088B97E8(&tmp, *ent);
    v.a = tmp.a;
    first = *(int *)&v;
    v.b = tmp.b;
    func_08AF6428(D_08B01CF8, &v, row, 0);
    i = 0;
    if (i < n) {
        do {
            ent = (unsigned short *)(self->b + row * 0x15F8 + *(unsigned short *)(self->b + row * 0x57E + i * 2 + 0x7EC0) * 8 + 0x26B8);
            if (func_08AF6BBC(first, *ent) != 0) {
                unsigned short keep = *(unsigned short *)(self->b + row * 0x57E + n * 2 + 0x7EC0);
                if (i < n) {
                    do {
                        *(unsigned short *)(self->b + row * 0x57E + n * 2 + 0x7EC0) = *(unsigned short *)(self->b + row * 0x57E + n * 2 + 0x7EBE);
                        n--;
                    } while (i < n);
                }
                *(unsigned short *)(self->b + row * 0x57E + i * 2 + 0x7EC0) = keep;
                return;
            }
            i++;
        } while (i < n);
    }
}
