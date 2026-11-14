struct Entry48 {
    char data[0x48];
};

struct EntrySet58FC {
    unsigned char active;
    char pad_01[0x3];
    Entry48 entries[3];
    char pad_DC[0x18];
    int field_F4;
};

extern "C" {

extern void func_089F5D04(EntrySet58FC *self);
extern void func_089F51C4(Entry48 *entry);

int func_089F58FC(EntrySet58FC *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->field_F4 == 0) {
        func_089F5D04(self);
    }
    for (int i = 0; i < 3; i++) {
        func_089F51C4(&self->entries[i]);
    }
    return 0;
}

}
