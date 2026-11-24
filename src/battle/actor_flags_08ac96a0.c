// Updates two flag bits (0x1 and 0x4) in the actor's status byte from the range check and the motion test; always returns 0.
struct Flags { char d0[7]; unsigned char b7; };
struct Actor { char d0[0x130]; char motion[0x480]; struct Flags flags; };
struct Rng { char d0[0x3C]; float a; float b; };
extern int func_08AC97A0(struct Rng *r, struct Actor *a, unsigned char x, unsigned char y, float a0, float b0);
extern int func_08AC94BC(struct Actor *a);
extern int func_088301D4(void *m);

int func_08AC96A0(struct Rng *r, struct Actor *a)
{
    float fa = r->a;
    float fb = r->b;
    struct Flags *f;
    int ok;
    if (a != 0) {
        ok = func_08AC97A0(r, a, 1, 1, fa, fb);
        f = &a->flags;
        if (ok != 0) {
            if (func_08AC94BC(a) == 0) {
                f->b7 = f->b7 | 1;
                if (func_088301D4(a->motion) == 1) {
                    f->b7 = f->b7 | 4;
                    goto done;
                }
                f->b7 = f->b7 & ~4;
                goto done;
            }
        }
        f->b7 = f->b7 & ~1;
        f->b7 = f->b7 & ~4;
    }
done:
    return 0;
}
