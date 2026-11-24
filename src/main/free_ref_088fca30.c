/* Release the linked resource if any: notify, tear down parts, free and clear both links. */
typedef struct Self { char pad0[0xC]; int a; char pad1[0x30 - 0x10]; int *link; } Self;
extern int D_089D6998;
extern void func_0887E0DC(int a, int v);
extern void func_088D3A04(int p);
extern void func_088FC588(Self *self);
extern void func_0885C538(int a, int *b, int c);
void func_088FCA30(Self *self)
{
    int *r = self->link;
    int *l;
    if (r == 0) {
        return;
    }
    if (r[0xEEC / 4] != -1) {
        func_0887E0DC(D_089D6998, 1);
        r = self->link;
    }
    func_088D3A04((int)r + 0x65C);
    func_088FC588(self);
    l = self->link;
    func_0885C538(self->a, l, 0);
    self->link = 0;
    self->a = 0;
}
