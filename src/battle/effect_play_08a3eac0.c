typedef struct EffectPlay3EAC0 {
    char pad_000[0x288];
    char field_288[0x4];
} EffectPlay3EAC0;

extern int func_088908E0(void);
extern void func_0887AC70(void *sub, int a, int b, int time);

void func_08A3EAC0(EffectPlay3EAC0 *self, int a, int b)
{
    func_0887AC70(self->field_288, a, b, func_088908E0());
}
