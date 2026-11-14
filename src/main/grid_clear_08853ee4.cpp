struct GridCell {
    char pad_00[0x10];
    float value;
    char pad_14[0xC];
};

struct Grid {
    char pad_00[0x10];
    GridCell cells[10];
    int field_150;
    int field_154;
};

extern "C" {

void func_08853EE4(Grid *self)
{
    for (int i = 0; i < 10; i++) {
        self->cells[i].value = 0.0f;
    }
    self->field_154 = 0;
    self->field_150 = 0;
}

}
