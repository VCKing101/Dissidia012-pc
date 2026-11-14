/* Marks the first free sub-entry (up to five) unless the group is locked. */
struct MarkEntry {
    int field_00;
    unsigned int field_04;
    int field_08;
    unsigned char field_0C;
    unsigned char field_0D;
    char pad_0E[0x18 - 0xE];
};

extern "C" {
void func_089F2110(void *self, MarkEntry *entries)
{
    for (int i = 0; i < 5; i++) {
        if (entries[i].field_0D == 0) {
            return;
        }
        {
            unsigned int flags = entries->field_04;
            if ((flags & 0x30) == 0 && (entries[i].field_0C & 1) == 0) {
                entries[i].field_0C = entries[i].field_0C | 1;
                entries->field_04 = flags | 0x100000;
                return;
            }
        }
    }
}
}
