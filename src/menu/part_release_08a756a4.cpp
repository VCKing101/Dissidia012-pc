struct ReleaseRow {
    char data[0x3C];
};

struct ReleasePart {
    int field_00[4];
    int field_10[3];
    ReleaseRow field_1C[16];
    char field_3DC[0x18];
    char field_3F4[4];
    char field_3F8[4];
};

extern "C" void func_08A73C7C(char *list);
extern "C" void func_08A73D98(char *list);
extern "C" void func_08A752C0(char *list);
extern "C" void func_08A74F10(ReleaseRow *row);
extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A756A4(ReleasePart *self)
{
    func_08A73C7C(self->field_3F4);
    func_08A73D98(self->field_3F8);
    func_08A752C0(self->field_3DC);
    for (int i = 0; i < 16; i++) {
        func_08A74F10(&self->field_1C[i]);
    }
    for (int i = 0; i < 4; i++) {
        func_088D2F30(&self->field_00[i]);
    }
    for (int i = 0; i < 3; i++) {
        func_088D3334(&self->field_10[i]);
    }
}
}
