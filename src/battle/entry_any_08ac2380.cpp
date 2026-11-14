typedef struct BusyEntry {
    char data[0xE0];
} BusyEntry;

typedef struct BusyOwner {
    unsigned char active;
    char pad_01[0x2B];
    int count;
    BusyEntry *entries;
} BusyOwner;

extern "C" {

extern int func_08AC12E8(BusyEntry *entry);

bool func_08AC2380(BusyOwner *self)
{
    if (!self->active) {
        return false;
    }
    for (int i = 0; i < self->count; i++) {
        if (func_08AC12E8(&self->entries[i]) == 1) {
            return true;
        }
    }
    return false;
}

}
