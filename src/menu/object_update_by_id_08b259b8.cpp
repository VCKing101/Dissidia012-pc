/* Menu object update: looks up the entry for an id in the page table and applies it to the matching object, or to all of them when the id is -1. */
struct Obj { char p[0x30]; int a30; int a34; int a38; };
struct W { char p0[4]; int objs[1]; };
struct Self { W *w; };
extern "C" {
unsigned short *func_08B28000(void);
void func_088D428C(void *, void *, unsigned short *, int);
void func_088D40BC(void *, void *, unsigned, unsigned, unsigned, unsigned);
void func_088D436C(void *, void *, unsigned short *);
void func_08B259B8(Self *self, int mode, unsigned id, int keep)
{
    unsigned short *e = func_08B28000();
    if (mode == 0) {
        if (id != 0xFFFFFFFF) {
            int i = 0;
            do {
                unsigned v = *e;
                if (v == id) {
                    W *w = self->w;
                    unsigned k = e[2];
                    if (keep == 0) {
                        Obj *o = *(Obj **)((char *)w + k * 4 + 4);
                        o->a34 = 0;
                        o->a30 = 0;
                        o->a38 = 0;
                        v = *e;
                        w = self->w;
                        k = e[2];
                    }
                    func_088D40BC((char *)w + 4, (char *)w + 0xEC, v, e[1], k, e[3]);
                    return;
                }
                i++;
                e += 4;
            } while (i < 0x2E);
        } else {
            func_088D428C((char *)self->w + 4, (char *)self->w + 0xEC, e, 1);
        }
    } else {
        func_088D436C((char *)self->w + 4, (char *)self->w + 0xEC, e);
    }
}
}
