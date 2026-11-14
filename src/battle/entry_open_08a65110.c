typedef struct EntryOwner {
    unsigned char active;
} EntryOwner;

extern int D_08AB8388;
extern void *func_08AB809C(int table, int id);
extern int func_08A652F4(EntryOwner *self, void *entry, unsigned char mode, int extra);

int func_08A65110(EntryOwner *self, int id, unsigned char mode, int extra)
{
    void *entry;
    if (!self->active) {
        return -1;
    }
    if (id < 0) {
        return -2;
    }
    entry = func_08AB809C(D_08AB8388, id);
    if (entry == 0) {
        return -3;
    }
    return func_08A652F4(self, entry, mode, extra);
}
