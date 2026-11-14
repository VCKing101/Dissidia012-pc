struct LinkTarget {
    char pad_000[0x14];
    unsigned char field_14;
};
struct LinkEntry {
    int id;
    LinkTarget *target;
};
struct Links4CA8 {
    unsigned char field_00;
    char pad_001[3];
    LinkEntry entries[20];
};
extern "C" {
int func_08A04CA8(Links4CA8 *self)
{
    if (!self->field_00) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        LinkEntry *e = &self->entries[i];
        if (e->target) {
            e->target->field_14 |= 1;
        }
    }
    return 0;
}
}
