typedef struct Kind1298 { char pad_000[0xC]; short field_0C; } Kind1298;
typedef struct Owner1298 { unsigned char field_00; char pad_001[7]; Kind1298 *field_08; } Owner1298;
int func_089F1298(Owner1298 *self)
{
    if (!self->field_00) {
        return 0;
    }
    if (!self->field_08) {
        return 0;
    }
    return self->field_08->field_0C == 2;
}
