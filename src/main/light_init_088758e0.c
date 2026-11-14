typedef struct Light { unsigned char on; unsigned char f01; char pad_02[2]; float f04; float f08; float f0C; float f10; unsigned char f14; char pad_15[3]; float f18; float f1C; } Light;
void func_088758E0(Light *self)
{
    self->on = 0;
    self->f01 = 0;
    self->f04 = 0.0f;
    self->f08 = 0.0f;
    self->f0C = 0.0f;
    self->f10 = 0.0f;
    self->f14 = 0;
    self->f18 = 0.0f;
    self->f1C = 0.0f;
}
