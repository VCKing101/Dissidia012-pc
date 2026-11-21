/* Picks a value of the current mode: the (skip+1)-th of the mode's values
 * that the table accepts (-1 when closed, out of range or none). */
struct PickEntry {
    int value;
    int pad_04;
};

struct PickInfo {
    int slot;
    PickEntry *entries;
    int count;
};

struct PickTable {
    unsigned char open;
    unsigned char pad_01[0xF];
    unsigned int mode;
};

extern PickInfo D_08A7FFE0[7];

extern "C" {

extern int func_08A7F338(PickTable *self, int value);

int func_08A7EC58(PickTable *self, unsigned int skip)
{
    if (!self->open) {
        return -1;
    }
    unsigned int mode = self->mode;
    if (mode >= 7 || skip >= 4) {
        return -1;
    }
    int found = -1;
    PickInfo *info = &D_08A7FFE0[mode];
    for (int i = 0; i < info->count; i++) {
        int value = info->entries[i].value;
        if (func_08A7F338(self, value) == 0) {
            continue;
        }
        if (skip == 0) {
            found = value;
            break;
        }
        skip--;
    }
    return found;
}

}
