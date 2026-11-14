struct Slot49BDC {
    int pad_00;
    int value;
};

extern "C" {
extern int D_089D4960;
extern Slot49BDC D_09B6DD08;
extern void func_08A49CE8(void);
extern void func_08A49D08(void);
extern void func_08844588(int o);
extern void func_0886503C(char *task, void (*first)(void), void (*second)(void), int flag);
extern void func_088650A4(char *task);
extern void func_0886504C(char *task, void *owner, int size);
}

extern "C" void func_08A49BDC(char *self, int value)
{
    func_08844588(D_089D4960);
    D_09B6DD08.value = value;
    func_0886503C(self + 0x100, func_08A49CE8, func_08A49D08, 0);
    func_088650A4(self + 0x100);
    func_0886504C(self + 0x100, self, 0x100);
}
