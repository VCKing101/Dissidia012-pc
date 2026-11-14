struct KindHolder;

struct HolderList {
    unsigned char active;
    char pad_01[0x3];
    int count;
    KindHolder **items;
};

extern "C" {

extern int func_08AB6A40(KindHolder *holder);
extern int func_08AB6A60(KindHolder *holder);

/* The first idle holder of kind, or 0. */
KindHolder *func_08AB7ABC(HolderList *self, int kind)
{
    if (self->active == 0) {
        return 0;
    }
    for (int i = 0; i < self->count; i++) {
        KindHolder *holder = self->items[i];
        if (func_08AB6A40(holder) != kind) {
            continue;
        }
        if (func_08AB6A60(holder) != 0) {
            continue;
        }
        return holder;
    }
    return 0;
}

}
