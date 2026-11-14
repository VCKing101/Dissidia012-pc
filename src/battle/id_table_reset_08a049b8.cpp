struct IdEntry {
    int id;
    int value;
};
struct Reset49B8 {
    unsigned char field_00;
    char pad_001[3];
    IdEntry entries[20];
};
extern "C" {
void func_08A049B8(Reset49B8 *self)
{
    self->field_00 = 0;
    for (int i = 0; i < 20; i++) {
        IdEntry *e = &self->entries[i];
        e->id = -1;
        e->value = 0;
    }
}
}
