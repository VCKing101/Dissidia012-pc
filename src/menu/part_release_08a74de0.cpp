struct ReleasePart {
    int field_00;
    int field_04[2];
};

extern "C" void func_088D3174(ReleasePart *self);
extern "C" void func_088D34B8(int *handle);

extern "C" {
void func_08A74DE0(ReleasePart *self)
{
    func_088D3174(self);
    for (int i = 0; i < 2; i++) {
        func_088D34B8(&self->field_04[i]);
    }
}
}
