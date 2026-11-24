/* If the current object has flag 0x10 and the handle check passes, clears the flags and the handle. */
struct Sub { char d[4]; unsigned flags; char e[0xB8]; int h; };
struct Cur { char d[0x44]; unsigned flags; char e[0x7C]; Sub *sub; };
extern Cur *D_08B41CC0;
extern "C" int func_089231D8(int);
struct H { static bool has(unsigned f) { return (f & 0x10) != 0; } };
extern "C" void func_08B3D2BC(void)
{
    Cur *c = D_08B41CC0;
    if (H::has(c->flags) && func_089231D8(c->sub->h) != 0) {
        c->flags &= ~0x10u;
        c->sub->flags &= ~4u;
        c->sub->h = 0;
    }
}
