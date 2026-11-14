/* Resets the marks block: four -1 ids, five empty entries and eight zero
   words. */
struct Entry08B35A64 {
    int field_0;
    unsigned char field_4;
    char pad_5[3];
};

struct Marks08B35A64 {
    int field_00[4];
    struct Entry08B35A64 field_10[5];
    int field_38[8];
};

void func_08B35A64(struct Marks08B35A64 *self)
{
    int i;
    self->field_00[0] = -1;
    self->field_00[1] = -1;
    self->field_00[2] = -1;
    self->field_00[3] = -1;
    for (i = 0; i < 5; i++) {
        struct Entry08B35A64 *entry = &self->field_10[i];
        entry->field_0 = 0;
        entry->field_4 = 0;
    }
    self->field_38[0] = 0;
    self->field_38[1] = 0;
    self->field_38[2] = 0;
    self->field_38[3] = 0;
    self->field_38[4] = 0;
    self->field_38[5] = 0;
    self->field_38[6] = 0;
    self->field_38[7] = 0;
}
