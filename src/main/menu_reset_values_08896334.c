extern void func_088A2664(int id, float value);
extern void func_088A24D4(int scope, int id, int value, int flags);
extern void func_08895F60(void *menu);

void func_08896334(void *menu)
{
    float zero = 0.0f;
    func_088A2664(0x18703, zero);
    func_088A2664(0x18704, zero);
    func_088A24D4(0, 0x18705, 1, 0);
    func_088A24D4(0, 0x18706, 1, 0);
    func_08895F60(menu);
}
