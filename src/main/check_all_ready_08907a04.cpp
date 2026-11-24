// If all three slots report the expected state, notifies the listener through virtual slot 6 and sets the mode to 6.
struct L { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); };
struct Slot { char d[0x2E0]; };
struct S { char d0[0x804]; int expect; Slot slots[3]; char d1[0x1644 - 0x808 - 3 * 0x2E0]; int flag; char d2[0x1658 - 0x1648]; int mode; int pad; L *listener; };
extern "C" int func_08906A98(Slot *s);
extern "C" void func_08907A04(S *self)
{
    int want;
    int ok;
    int i;
    Slot *p;
    want = self->flag != 0 ? 1 : self->expect;
    ok = 1;
    for (i = 0, p = self->slots; i < 3; i++, p++) {
        if (func_08906A98(p) != want) { ok = 0; break; }
    }
    if (ok == 1) {
        if (self->listener) self->listener->v6();
        self->mode = 6;
    }
}
