typedef struct Mixer { int handle; char pad_04[0x32C]; int field_330; int field_334; unsigned char field_338; char pad_339[3]; int field_33C; int field_340; float field_344; } Mixer;
extern void func_0899BB1C(int handle);
void func_08858704(Mixer *self)
{
    func_0899BB1C(self->handle);
    self->handle = 0;
    self->field_334 = 0;
    self->field_330 = 0;
    self->field_340 = 0;
    self->field_33C = 0;
    self->field_344 = 1.0f;
}
