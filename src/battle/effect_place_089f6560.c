typedef struct EffectPlace {
    unsigned char active;
    char pad_01[0x3];
    char field_04[0x4];
} EffectPlace;

extern unsigned char D_08AEB9C0;
extern void func_08AEB044(void *sub, int id, float x, float y, float z);
extern void func_08A23154(int mode);

int func_089F6560(EffectPlace *self, int id, float x, float y, float z)
{
    if (!self->active) {
        return -1;
    }
    func_08AEB044(self->field_04, id, x, y, z);
    if (D_08AEB9C0 == 1) {
        func_08A23154(2);
    }
    return 0;
}
