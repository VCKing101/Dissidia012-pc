typedef struct Mode4C2B8 {
    char pad_00[0x11];
    unsigned char flag;
} Mode4C2B8;

extern int D_089D4960;
extern Mode4C2B8 D_09B6DCE8;
extern void func_08844588(int o);
extern void func_08A4C304(int self);

void func_08A4C2B8(int self, unsigned char flag)
{
    func_08844588(D_089D4960);
    D_09B6DCE8.flag = flag;
    func_08A4C304(self);
}
