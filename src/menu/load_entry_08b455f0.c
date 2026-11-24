/* Loads the data file for list entry `idx` (unless it is already loaded) into a temporary buffer and hands it to the entry. */
extern int func_08B45424(void *entry);
extern void *func_088FE7C8(const char *name, int a);
extern void *func_0885C6CC(void *heap, void *id, int size, int c, int d, int e);
extern void *func_088FE708(const char *name, void *buf, void *id, int a);
extern void func_088FE944(const char *name, void *buf, void *r);
extern void func_08B45298(void *entry, void *buf, void *id);
extern void func_0885C538(void *heap, void *buf, int size);

typedef struct L455F0 { void *heap; void **tab; } L455F0;

void func_08B455F0(L455F0 *self, int idx, const char *name)
{
    void *id;
    void *buf;
    void *r;
    if (func_08B45424(self->tab[idx]) != 0) {
        return;
    }
    id = func_088FE7C8(name, 0);
    buf = func_0885C6CC(self->heap, id, 0x40, 0, 1, 0);
    r = func_088FE708(name, buf, id, 0);
    func_088FE944(name, buf, r);
    func_08B45298(self->tab[idx], buf, id);
    func_0885C538(self->heap, buf, 0x40);
}
