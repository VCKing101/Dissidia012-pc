/* Resets the block at D_09AFB8A0: func_088FAE70(0..1), func_088F9054 unless the word at +0x25F8 is set, func_088F90F4, state byte 4, +0x25B3 = 0xFF, clears the fields and +0x2784..0x278F, then func_088FB3C0, func_088FB434(0), func_088FB5D0(0, 1, 1, 1, 1). */
struct Sub088f9788 { unsigned char pad0[8]; unsigned char state; unsigned char pad1[2]; unsigned char b3; unsigned char pad2[0x50 - 0xC]; unsigned flags; };
struct Blk088f9788 {
    unsigned char pad0[0x25A8];
    Sub088f9788 sub;
    unsigned char pad3[0x2784 - 0x25A8 - sizeof(Sub088f9788)];
    unsigned short h0, h2; int w;
    unsigned short h4, h6;
};
extern "C" Blk088f9788 D_09AFB8A0;
extern "C" void func_088FAE70(int i);
extern "C" void func_088F9054(Blk088f9788 *b);
extern "C" void func_088F90F4(Blk088f9788 *b);
extern "C" void func_088FB3C0(void);
extern "C" void func_088FB434(int v);
extern "C" void func_088FB5D0(int a, int b, int c, int d, int e);
extern "C" void func_088F9788(void)
{
    Sub088f9788 *s = &D_09AFB8A0.sub;
    for (int i = 0; i < 2; i++) {
        func_088FAE70(i);
    }
    {
        int one = 1;
        if ((unsigned char)((s->flags & 1) == 0) == one) {
            func_088F9054(&D_09AFB8A0);
        }
    }
    func_088F90F4(&D_09AFB8A0);
    s->state = 4;
    s->b3 = 0xFF;
    D_09AFB8A0.h0 = 0;
    D_09AFB8A0.h2 = 0;
    D_09AFB8A0.w = 0;
    D_09AFB8A0.h4 = 0;
    D_09AFB8A0.h6 = 0;
    func_088FB3C0();
    func_088FB434(0);
    func_088FB5D0(0, 1, 1, 1, 1);
}
