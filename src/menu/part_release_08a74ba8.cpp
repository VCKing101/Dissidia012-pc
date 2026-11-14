struct ReleasePart {
    int field_00;
    int field_04[2];
};

extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A74BA8(ReleasePart *self)
{
    func_088D2F30(&self->field_00);
    for (int i = 0; i < 2; i++) {
        func_088D3334(&self->field_04[i]);
    }
}
}
