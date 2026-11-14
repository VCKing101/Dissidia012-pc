struct UpdateRow {
    char data[0x20];
};

struct UpdateRows {
    UpdateRow field_00[3];
    int field_60;
};

extern "C" void func_08A7697C(UpdateRow *row);

extern "C" {
void func_08A76D60(UpdateRows *self)
{
    for (int i = 0; i < self->field_60; i++) {
        func_08A7697C(&self->field_00[i]);
    }
}
}
