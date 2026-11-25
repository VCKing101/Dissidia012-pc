/* Calls the owner's virtual hook, then allocates and registers 0xB8 row handles (0x5D from the first pool, the rest from the second) and logs the table. */
typedef struct Ent5F710 { short adj; short pad; void (*fn)(void *); } Ent5F710;
typedef struct Own5F710 { char pad[0x48]; Ent5F710 e; } Own5F710;
typedef struct S5F710 { char pad0[0x24]; Own5F710 *own; char pad1[0x30 - 0x28]; int pool0; int pool1; int padx; int row[0xB8]; } S5F710;
extern int func_088DAA68(int);
extern void func_088DAB7C(int, int);
extern void func_088D3CF0(int, void *, void *);
extern char D_08A5FA68[];
void func_08A5F710(S5F710 *self)
{
    int i;
    int *p;
    Ent5F710 *e = &self->own->e;
    e->fn((char *)self + e->adj);
    for (i = 0, p = (int *)self; i < 0x5D; i++, p++) {
        int h;
        int q;
        h = func_088DAA68(self->pool0);
        q = self->pool0;
        p[0x3C / 4] = h;
        func_088DAB7C(q, h);
    }
    for (i = 0x5D, p = (int *)self + 0x174 / 4; i < 0xB8; i++, p++) {
        int h;
        int q;
        h = func_088DAA68(self->pool1);
        q = self->pool1;
        p[0x3C / 4] = h;
        func_088DAB7C(q, h);
    }
    func_088D3CF0(0, self->row, D_08A5FA68);
}
