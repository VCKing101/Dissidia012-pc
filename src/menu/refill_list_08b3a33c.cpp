// Resets the owner's list, then re-adds every entry of the pool to the sub-object by index.
struct Sub { char pad[0x14]; int h; };
struct Owner { char pad0[0xB8]; int list; char pad1[0xC]; Sub *sub; };
extern char *D_089D712C;
extern "C" {
extern void func_08B44F40(int);
extern int *func_089200D8(char *);
extern int *func_089200E0(char *, int);
extern void func_08930E10(int, int, int);
void func_08B3A33C(Owner *self) {
    char *pool = D_089D712C + 0x3F548;
    func_08B44F40(self->list);
    for (int i = 0; i < *func_089200D8(pool); i++) {
        int h = self->sub->h;
        int *e = func_089200E0(pool, i);
        func_08930E10(h, i, *e);
    }
}
}
