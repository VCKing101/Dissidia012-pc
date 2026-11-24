/* Frees the two icon handles, picks a resource id from the mode fields and recreates the handles, then re-registers. */
extern void func_0887BA5C(void *h);
extern void *func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern void *func_0887E534(void *h);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern void func_08B4DDA8(void *self);
extern const char D_08B4EBB8[];

typedef struct S4DDF8 {
    char pad0[0x10];
    char a10[0x30];
    char a40[0x2C];
    void *h6C;
    char pad70[0x18];
    void *h88;
    char pad8C[8];
    int f94;
    int f98;
    int f9C;
    unsigned char fA0;
    unsigned char fA1;
} S4DDF8;

void func_08B4DDF8(S4DDF8 *self)
{
    int id;
    if (self->h6C != 0) {
        func_0887BA5C(self->h6C);
        self->h6C = 0;
    }
    if (self->h88 != 0) {
        func_0887BA5C(self->h88);
        self->h88 = 0;
    }
    id = -1;
    if (self->fA1 == 0) {
        id = 0x1D;
    } else {
        switch (self->f9C) {
        case 0:
            id = 0x1E;
            break;
        case 1:
            id = 0x1F;
            break;
        }
    }
    if (id != -1) {
        void *h = func_0887B3EC(self->f94, self->f98, 0, id, 0, 0);
        self->h6C = h;
        self->h88 = func_0887E534(h);
    }
    func_088D428C(self->a10, self->a40, D_08B4EBB8, 1);
    func_08B4DDA8(self);
}
