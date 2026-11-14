typedef struct TargetLink {
    char pad_00[0x4];
    void *target;
} TargetLink;

typedef struct EntryOwner {
    unsigned char active;
    char pad_01[0x3];
    TargetLink *link;
} EntryOwner;

extern int D_08AB8388;
extern void *func_08AB809C(int table, int id);
extern int func_08A7BF1C(EntryOwner *self, void *entry);

int func_08A7BB0C(EntryOwner *self, int id)
{
    void *entry;
    if (!self->active) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    if (self->link->target == 0) {
        return 0;
    }
    entry = func_08AB809C(D_08AB8388, id);
    if (entry == 0) {
        return 0;
    }
    return func_08A7BF1C(self, entry);
}
