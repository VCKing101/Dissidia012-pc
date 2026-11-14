struct IdEntry {
    int id;
    int value;
};
struct Reset3910 {
    unsigned char field_00;
    char pad_001[3];
    int field_04;
    IdEntry entries[20];
    int field_A8;
};
extern "C" {
void func_08A03910(Reset3910 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    for (int i = 0; i < 20; i++) {
        IdEntry *e = &self->entries[i];
        e->id = -1;
        e->value = 0;
    }
    self->field_A8 = 0;
}
}
