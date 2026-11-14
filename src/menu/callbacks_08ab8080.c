typedef struct FlagGlobal {
    char pad_00[0xA];
    char field_0A;
} FlagGlobal;

extern void func_0886503C(int target, void (*a)(void), void (*b)(void), int c);
extern void func_0886504C(int target, int owner, int size);
extern void func_08AB9BB8(void);
extern void func_08AB9BD8(void);
extern FlagGlobal D_09B6D818;

void func_08AB8080(int self)
{
    func_0886503C(self + 0x100, func_08AB9BB8, func_08AB9BD8, 0);
    func_0886504C(self + 0x100, self, 0x100);
    D_09B6D818.field_0A = 0;
}
