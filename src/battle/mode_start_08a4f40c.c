typedef struct ModeStart4F40C {
    char pad_000[0x8];
    int field_08;
} ModeStart4F40C;

extern void func_08A4F454(ModeStart4F40C *self);

void func_08A4F40C(ModeStart4F40C *self)
{
    if (self->field_08 == 0) {
        func_08A4F454(self);
    }
}
