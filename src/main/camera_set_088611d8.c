typedef struct Camera { char pad_00[0x434]; float field_434; float field_438; } Camera;
void func_088611D8(float value, Camera *self)
{
    self->field_434 = value;
    self->field_438 = 0.0f;
}
