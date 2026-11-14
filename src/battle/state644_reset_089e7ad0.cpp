struct State644 {
    char field_00;
    char pad_001[0x643];
    char field_644[2];
    char pad_646[2];
    int field_648;
    int field_64C;
};

extern "C" {
void func_089E7AD0(State644 *self)
{
    self->field_00 = 0;
    for (int i = 0; i < 2; i++) {
        *((char *)self + 0x644 + i) = 0;
    }
    self->field_648 = 0;
    self->field_64C = 0;
}
}
