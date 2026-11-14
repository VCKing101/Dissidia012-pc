struct ReleasePart {
    int field_00;
    int field_04[2];
};

extern "C" void func_088D30E4(ReleasePart *self, int *handle);
extern "C" void func_088D3334(int *handle);

extern "C" {
void func_08A74D84(ReleasePart *self)
{
    for (int i = 0; i < 2; i++) {
        func_088D30E4(self, &self->field_04[i]);
        func_088D3334(&self->field_04[i]);
    }
}
}
