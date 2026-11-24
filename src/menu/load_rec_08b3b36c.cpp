// Looks up the source for slot idx (8-byte stride, at +0x10), builds a stack record from it and copies the record to dst.
struct RecA { int w[0x1DE]; };
struct RecB { int w[12]; };
struct Slot { int v; int pad; };
struct Tab { char pad[0x10]; Slot s[1]; };
struct Blk { char pad[0xC8]; char x[1]; };
extern Blk D_09B6DAC0;
extern "C" {
extern void func_088CB9E8(RecA *);
extern void func_088C5E44(RecB *);
extern void func_08931A1C(int, RecA *, RecB *, char *);
void func_08B3B36C(RecA *dst, Tab *tab, int idx) {
    RecA ra;
    RecB rb;
    int src = tab->s[idx].v;
    func_088CB9E8(&ra);
    func_088C5E44(&rb);
    func_08931A1C(src, &ra, &rb, D_09B6DAC0.x);
    *dst = ra;
}
}
