struct ReleasePart {
    int field_00[3];
    int field_0C;
    char field_10[0x68];
    char field_78[4];
};

extern "C" void func_08A73C7C(char *list);
extern "C" void func_08A76D60(char *rows);
extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A770FC(ReleasePart *self)
{
    func_08A73C7C(self->field_78);
    func_08A76D60(self->field_10);
    for (int i = 0; i < 3; i++) {
        func_088D2F30(&self->field_00[i]);
    }
    func_088D3334(&self->field_0C);
}
}
