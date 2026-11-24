// When the 0x20 flag of the object at +0x34 is set, calls the adjusted method of the table at +0x24 and advances state to 2.
struct F { char pad[0x20]; unsigned short flags; static bool has(unsigned v, unsigned m) { return (v & m) != 0; } };
struct Pmf { short adj; short pad; void (*fn)(void *); };
struct V { char pad[0x28]; Pmf m; };
struct S { char pad[0x24]; V *vt; int state; char pad2[8]; F *f; };
extern "C" void func_0892F93C(S *self) {
    if (!F::has(self->f->flags, 0x20)) return;
    Pmf *m = &self->vt->m;
    m->fn((char *)self + m->adj);
    self->state = 2;
}
