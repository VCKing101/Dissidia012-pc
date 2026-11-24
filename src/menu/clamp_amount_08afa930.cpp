// Returns the amount available (a limit global, or the stored total minus a looked-up item's cost) clamped to 0..999999.
struct Entry { char pad[0xC]; int cost; };
struct Inner { int first; char pad[0xC10 - 4]; int total; };
struct Owner { char pad[0x140]; Inner *inner; };
struct Glob { char pad[0xE8]; int lim; };
extern Glob D_09B6CDA0;
extern "C" {
extern void func_088CB9E8(char *);
extern Entry *func_08AF9720(char *, int);
int func_08AFA930(Owner *self) {
    char buf[1912];
    int v;
    if (D_09B6CDA0.lim >= 0) {
        v = D_09B6CDA0.lim;
        if (self->inner->total < v) {
            v = self->inner->total;
        }
    } else {
        int total = self->inner->total;
        func_088CB9E8(buf);
        Entry *e = func_08AF9720(buf, self->inner->first);
        v = total - e->cost;
    }
    if (v < 0) {
        v = 0;
    } else if (v > 999999) {
        v = 999999;
    }
    return v;
}
}
