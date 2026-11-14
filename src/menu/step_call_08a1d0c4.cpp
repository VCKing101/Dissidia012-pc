struct StepCtx {
    char pad_0000[0xD84];
    int field_D84;
    char pad_D88[0x6964 - 0xD88];
    int field_6964;
};

struct StepOwner {
    char pad_000[0x154];
    StepCtx *field_154;
};

extern "C" {

void func_088F795C();
void func_08A1CF60(StepOwner *self);
unsigned char func_08A1D8E4(StepOwner *self);

void func_08A1D0C4(StepOwner *self)
{
    if (self->field_154->field_D84 == 3) {
        return;
    }
    if (self->field_154->field_6964 != 0) {
        func_088F795C();
        if (func_08A1D8E4(self) != 0) {
            func_08A1CF60(self);
        }
    }
}

}
