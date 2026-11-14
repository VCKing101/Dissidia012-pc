/* True when the fade has reached its target (field 0xB0 <= field 0xAC). */
struct Fade08B34374 {
    char pad_00[0xAC];
    float field_AC;
    float field_B0;
};

unsigned char func_08B34374(struct Fade08B34374 *self)
{
    if (self->field_B0 <= self->field_AC) {
        return 1;
    }
    return 0;
}
