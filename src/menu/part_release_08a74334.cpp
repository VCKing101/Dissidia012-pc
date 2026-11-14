struct ReleasePart {
    int field_00[3];
    int field_0C;
};

extern "C" void func_088D2F30(int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A74334(ReleasePart *self)
{
    for (int i = 0; i < 3; i++) {
        func_088D2F30(&self->field_00[i]);
    }
    func_088D3334(&self->field_0C);
}
}
