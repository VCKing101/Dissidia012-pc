/* Draws the label, then each used icon slot (two models and the handle). */
struct IconSlot {
    void *field_00;
    void *field_04;
    void *field_08;
};

struct IconSlots {
    char pad_00[0x4];
    int field_04;
    IconSlot field_08[8];
    char pad_68[0x4];
    int field_6C;
};

extern "C" {
void func_088D9F5C(int label, int value, int redraw);
void func_0887B924(void *model);
void func_088D9CD8(void *handle);

void func_089EB7D0(IconSlots *self)
{
    func_088D9F5C(self->field_04, 10, 1);
    for (int i = 0; i < self->field_6C; i++) {
        IconSlot *slot = &self->field_08[i];
        if (slot->field_04 != 0) {
            func_0887B924(slot->field_04);
        }
        if (slot->field_08 != 0) {
            func_0887B924(slot->field_08);
        }
        void *handle = slot->field_00;
        if (handle != 0) {
            func_088D9CD8(handle);
        }
    }
}
}
