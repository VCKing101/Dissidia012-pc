/* Releases the five icon slots (one handle, two models each) and the shared handle. */
struct IconSlot {
    void *field_00;
    void *field_04;
    void *field_08;
};

struct IconSlots {
    char pad_00[0x4];
    void *field_04;
    IconSlot field_08[5];
};

extern "C" {
void func_088DA060(void *handle);
void func_0887BA5C(void *model);

void func_089EB71C(IconSlots *self)
{
    for (int i = 0; i < 5; i++) {
        IconSlot *slot = &self->field_08[i];
        if (slot->field_00 != 0) {
            func_088DA060(slot->field_00);
        }
        if (slot->field_04 != 0) {
            func_0887BA5C(slot->field_04);
        }
        if (slot->field_08 != 0) {
            func_0887BA5C(slot->field_08);
        }
        slot->field_00 = 0;
        slot->field_04 = 0;
        slot->field_08 = 0;
    }
    if (self->field_04 != 0) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
}
}
