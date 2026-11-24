/* Runs the sub cleanups, releases the paired handles in each 8-byte entry, then the final cleanup and mixer reset. */
extern "C" {
struct Entry { int a; int b; };
struct Obj { char pad[0xC]; Entry entries[3]; int n; };
extern void *D_089D7830;
extern void func_08AF3C94(void *p);
extern void func_08AF8478(void *p);
extern void func_08AF84D0(void *p);
extern void func_088DA060(void);
extern void func_088DAB44(void);
extern void func_088D0370(void *mixer, int id, unsigned char flag);

void func_08AF74AC(Obj *self)
{
    unsigned char zero = 0;
    func_08AF3C94(self);
    func_08AF8478(self);
    for (int i = 0; i < self->n; i++) {
        Entry *e = &self->entries[i];
        if (e->b != 0) {
            func_088DA060();
            e->b = 0;
        }
        if (e->a != 0) {
            func_088DAB44();
            e->a = 0;
        }
    }
    func_08AF84D0(self);
    int id = 0xD;
    func_088D0370(D_089D7830, id, zero);
}
}
