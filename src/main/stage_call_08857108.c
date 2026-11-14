typedef struct Stage { char pad_00[0x69C0]; char field_69C0[4]; } Stage;
extern Stage *D_089D4DC8;
extern void func_08857134(void *dst, int value);
void func_08857108(int unused, short value)
{
    func_08857134(D_089D4DC8->field_69C0, value);
}
