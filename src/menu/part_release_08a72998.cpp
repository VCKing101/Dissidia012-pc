struct ReleasePart {
    int field_00[5];
    int field_14[1];
    char field_18[4];
};

extern "C" void func_08A73C7C(char *list);
extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A72998(ReleasePart *self)
{
    func_08A73C7C(self->field_18);
    for (int i = 0; i < 5; i++) {
        func_088D2F30(&self->field_00[i]);
    }
    for (int i = 0; i < 1; i++) {
        func_088D3334(&self->field_14[i]);
    }
}
}
