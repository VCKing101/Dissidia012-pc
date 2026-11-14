struct ReleasePart {
    int field_00;
    int field_04[2];
};

extern "C" void func_088D319C(ReleasePart *self);
extern "C" void func_088D34E0(int *handle);

extern "C" {
void func_08A74E30(ReleasePart *self)
{
    func_088D319C(self);
    for (int i = 0; i < 2; i++) {
        func_088D34E0(&self->field_04[i]);
    }
}
}
