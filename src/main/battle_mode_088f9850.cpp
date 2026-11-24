/* Sets the state byte at D_09AFB8A0 + 0x25B0 and +0x25B3 (flag 1: 6/0, else 3/0xFF), clears the fields at +0x2784..0x278F, then func_088FB3C0, func_088FB434(a), func_088FB5D0(0, 1, 1, 1, 1). */
struct Sub088f9850 { unsigned char pad0[8]; unsigned char state; unsigned char pad1[2]; unsigned char b3; };
struct Blk088f9850 {
    unsigned char pad0[0x25A8];
    Sub088f9850 sub;
    unsigned char pad3[0x2784 - 0x25A8 - sizeof(Sub088f9850)];
    unsigned short h0, h2; int w;
    unsigned short h4, h6;
};
extern "C" Blk088f9850 D_09AFB8A0;
extern "C" void func_088FB3C0(void);
extern "C" void func_088FB434(int v);
extern "C" void func_088FB5D0(int a, int b, int c, int d, int e);
extern "C" void func_088F9850(int a, unsigned char flag)
{
    Sub088f9850 *s = &D_09AFB8A0.sub;
    if (flag == 1) {
        s->state = 6;
        s->b3 = 0;
    } else {
        s->state = 3;
        s->b3 = 0xFF;
    }
    D_09AFB8A0.h0 = 0;
    D_09AFB8A0.h2 = 0;
    D_09AFB8A0.w = 0;
    D_09AFB8A0.h4 = 0;
    D_09AFB8A0.h6 = 0;
    func_088FB3C0();
    func_088FB434(a);
    func_088FB5D0(0, 1, 1, 1, 1);
}
