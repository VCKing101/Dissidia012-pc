typedef struct Self9FEC { char pad[0x138]; int sub; } Self9FEC;
extern void func_0887BA5C();
extern int func_08AB9B84(void *, unsigned);
extern int func_088F45DC(int);
extern int func_08AB8150(void *, unsigned);
extern int func_08AB8128(void *, unsigned);
extern int func_0887AD78();
extern int func_0887E534();
#define S (self->sub)

void func_08AB9FEC(Self9FEC *self, int p2)
{
    int off;
    unsigned id;
    int h;
    int ix;
    int b;
    int e;
    int r;
    off = p2 * *(int *)(S + 0xD34);
    id = p2 + *(int *)(S + 0x8BC);
    h = func_08AB9B84(self, id);
    ix = func_088F45DC(h);
    b = S + off * 2;
    if (*(unsigned short *)(b + 0x980) != id) {
        *(short *)(b + 0x980) = (short)id;
        off = off * 4;
        b = S + off;
        if (*(int *)(b + 0x900) != 0) {
            func_0887BA5C(*(int *)(b + 0x900));
            *(int *)(S + off + 0x900) = 0;
            b = S + off;
        }
        if (*(int *)(b + 0x904) != 0) {
            func_0887BA5C(*(int *)(b + 0x904));
            *(int *)(S + off + 0x904) = 0;
            b = S + off;
        }
        if (*(int *)(b + 0x908) != 0) {
            func_0887BA5C(*(int *)(b + 0x908));
            *(int *)(S + off + 0x908) = 0;
            b = S + off;
        }
        if (*(int *)(b + 0x90C) != 0) {
            func_0887BA5C();
            *(int *)(S + off + 0x90C) = 0;
        }
        e = func_08AB8150(self, id);
        h = func_0887AD78(*(int *)(S + 0x8FC), 0, *(unsigned short *)(e + 4), 0, 0, 0);
        *(int *)(S + off + 0x900) = h;
        h = func_0887E534(*(int *)(S + off + 0x900));
        *(int *)(S + off + 0x904) = h;
        r = *(int *)(S + off + 0x904);
        *(unsigned *)(r + 0x10) = *(unsigned *)(r + 0x10) | 0x800;
        r = func_08AB8128(self, id);
        if (r != 0) {
            h = func_0887AD78(*(int *)(S + 0x8FC), 0, *(unsigned short *)(e + *(unsigned char *)(S + ix + 0xAB4) * 4 + 8), 0, 0, 0);
            *(int *)(S + off + 0x908) = h;
            h = func_0887E534(*(int *)(S + off + 0x908));
            *(int *)(S + off + 0x90C) = h;
            off = *(int *)(S + off + 0x90C);
            *(unsigned *)(off + 0x10) = *(unsigned *)(off + 0x10) | 0x800;
        }
    }
}
