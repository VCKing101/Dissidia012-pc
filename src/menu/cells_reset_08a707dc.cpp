struct ResetCell {
    char data[6];
};

struct ResetCells {
    ResetCell field_00[5];
    short field_1E;
    char pad_20[0x18];
    short field_38;
};

extern "C" void func_08A707BC(ResetCell *cell);

extern "C" {
void func_08A707DC(ResetCells *self)
{
    for (int i = 0; i < 5; i++) {
        func_08A707BC(&self->field_00[i]);
    }
    self->field_1E = 0;
    self->field_38 = 0;
}
}
