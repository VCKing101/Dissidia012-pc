typedef struct ModePick4F1F8 {
    char pad_000[0x8];
    int field_08;
} ModePick4F1F8;

extern int func_08A50140(void);

int func_08A4F1F8(ModePick4F1F8 *self)
{
    if (self->field_08 == 0) {
        if (!func_08A50140()) {
            return 7;
        }
        return 9;
    }
    return 0;
}

int func_08A4F23C(void)
{
    if (!func_08A50140()) {
        return 0x1F40;
    }
    return 0x1FA6;
}
