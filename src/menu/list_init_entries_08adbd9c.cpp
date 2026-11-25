/* Menu list init: numbers the entries, runs the two per-entry setups and, when the extra flag is set, the third setup. */
struct W { char p0[0x1C34]; int flag; char p1[0x78]; int count; char p2[0x1414]; short order[1]; };
struct Self { char p[0x46AC]; W *w; };
extern "C" {
void func_08ADBC84(Self *, int);
void func_08ADBAB8(Self *, int);
void func_08ADBD9C(Self *self)
{
    int i;
    for (i = 0; i < self->w->count; i++) {
        self->w->order[i] = i;
    }
    for (i = 0; i < self->w->count; i++) {
        func_08ADBC84(self, i);
    }
    if (self->w->flag != 0) {
        for (i = 0; i < self->w->count; i++) {
            func_08ADBAB8(self, i);
        }
    }
}
}
