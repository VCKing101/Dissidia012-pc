/* Initialises the five rows, the two icons and the two labels. */
struct InitRow {
    char data[0x14];
};

struct RowInitPanel {
    char field_00[2][4];
    char field_08[2][4];
    InitRow field_10[5];
};

extern "C" void func_08A75BFC(InitRow *row);
extern "C" void func_088D2F30(void *icon);
extern "C" void func_088D3334(void *label);

extern "C" {
void func_08A75E7C(RowInitPanel *self)
{
    for (int i = 0; i < 5; i++) {
        func_08A75BFC(&self->field_10[i]);
    }
    for (int j = 0; j < 2; j++) {
        char *icon = self->field_00[j];
        func_088D2F30(icon);
    }
    for (int j = 0; j < 2; j++) {
        func_088D3334(self->field_08[j]);
    }
}
}
