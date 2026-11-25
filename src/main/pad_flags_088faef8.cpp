struct Flags088FAEF8 { unsigned short bits; unsigned char b2; unsigned char b3; };
extern "C" int func_088F47E0(int);
extern "C" void func_088FAEF8(Flags088FAEF8 *p);
extern "C" void func_088FAEF8(Flags088FAEF8 *p)
{
    p->bits = 0;
    unsigned short old = p->bits;
    int one = 1;
    int r = func_088F47E0(1);
    p->bits = old | (r == 0);
    old = p->bits;
    unsigned short add = (func_088F47E0(2) == 0) ? 2 : 0;
    p->bits = old | add;
    old = p->bits;
    add = ((func_088F47E0(3)) == 0) ? 4 : 0;
    p->bits = old | add;
    old = p->bits;
    add = ((func_088F47E0(9)) == one) ? 8 : 0;
    p->bits = old | add;
    old = p->bits;
    add = ((func_088F47E0(0x1A)) == 0) ? 0x10 : 0;
    p->bits = old | add;
    old = p->bits;
    add = ((func_088F47E0(4)) == 0) ? 0x20 : 0;
    p->bits = old | add;
    old = p->bits;
    add = ((func_088F47E0(0x10)) == 0) ? 0x40 : 0;
    p->bits = old | add;
    p->b2 = func_088F47E0(5);
    p->b3 = func_088F47E0(0x18);
}
