// Adds an entry to the pool and the list, clears the selection, and optionally registers it with the sub-object.
struct Sub { char pad[0x14]; int h; };
struct Owner { char pad0[0xB8]; int list; char pad1[0xC]; Sub *sub; };
struct Blk { char pad[4]; short sel; };
extern char *D_089D712C;
extern Blk D_09B6DAC0;
extern "C" {
extern void func_0892017C(char *, int *);
extern void func_08B45000(int, int *);
extern void func_08930EA0(int, int *);
void func_08B3A298(Owner *self, int *entry, unsigned char flag) {
    if (*entry == -1) {
        return;
    }
    func_0892017C(D_089D712C + 0x3F548, entry);
    func_08B45000(self->list, entry);
    D_09B6DAC0.sel = -1;
    if (flag != 0) {
        func_08930EA0(self->sub->h, entry);
    }
}
}
