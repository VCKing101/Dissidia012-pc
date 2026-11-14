typedef struct LookupOwner {
    unsigned char active;
} LookupOwner;

extern int D_08AB8388;
extern void *func_08AB809C(int table, int id);
extern int func_08A7BEC0(LookupOwner *self, void *entry);

int func_08A7BA94(LookupOwner *self, int id)
{
    void *entry;

    if (self->active == 0) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    entry = func_08AB809C(D_08AB8388, id);
    if (entry == 0) {
        return 0;
    }
    return func_08A7BEC0(self, entry);
}
