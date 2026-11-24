// Returns whether the distance between two cast positions is at most the float at +4 of entry idx.
extern "C" float func_089A3C18(void *a, void *b);
struct Cast08B0EB78 { char pad[0x38]; char *a; char *b; };
extern "C" Cast08B0EB78 *D_089D6A10;
struct Rec08B0EB78 { int pad; float lim; };
struct Slot08B0EB78 { Rec08B0EB78 *r; int pad; };
struct Check08B0EB78 {
    static int Ge(float limit, float dist) { return dist <= limit; }
    static unsigned char run(Slot08B0EB78 *slots, int idx)
    {
        float *p = &slots[idx].r->lim;
        float d = func_089A3C18(D_089D6A10->a + 0x80, D_089D6A10->b + 0x80);
        return Ge(*p, d);
    }
};
extern "C" unsigned char func_08B0EB78(Slot08B0EB78 *slots, int idx) { return Check08B0EB78::run(slots, idx); }
