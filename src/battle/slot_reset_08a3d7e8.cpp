typedef struct SlotReset {
    char pad_000[0x21C];
    float field_21C;
    char pad_220[0x30];
    char field_250[0x28];
    int field_278;
    char pad_27C[0x1C];
    float field_298;
} SlotReset;

extern "C" {

extern void func_08935828(void *dest, int value, int size);
extern void *func_08A3D720(SlotReset *self, int index);
extern void func_08A40670(void *slot);

void func_08A3D7E8(SlotReset *self)
{
    int i;
    self->field_278 = 0;
    self->field_298 = 0.0f;
    self->field_21C = 0.0f;
    func_08935828(self->field_250, 0, 0x1C);
    for (i = 0; i < 13; i++) {
        func_08A40670(func_08A3D720(self, i));
    }
}

}
