struct ReleaseRow {
    char data[0x18];
};

struct ReleaseList {
    int field_00[5];
    int field_14[6];
    ReleaseRow field_2C[10];
    char field_11C[4];
    char field_120[4];
};

extern "C" void func_08A73C7C(char *list);
extern "C" void func_08A73D98(char *list);
extern "C" void func_08A73188(ReleaseRow *row);
extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A73770(ReleaseList *self)
{
    func_08A73C7C(self->field_11C);
    func_08A73D98(self->field_120);
    for (int i = 0; i < 10; i++) {
        func_08A73188(&self->field_2C[i]);
    }
    for (int i = 0; i < 5; i++) {
        func_088D2F30(&self->field_00[i]);
    }
    for (int i = 0; i < 6; i++) {
        func_088D3334(&self->field_14[i]);
    }
}
}
