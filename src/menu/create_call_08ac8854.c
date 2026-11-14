typedef struct CreateGlobal {
    char pad_000[0x134];
    int *field_134;
} CreateGlobal;

extern int func_0885C6CC(int *target, int a, int b, int c, int d, char e);
extern CreateGlobal *D_08AC7500;

void func_08AC8854(int a)
{
    int none = 0;
    func_0885C6CC(D_08AC7500->field_134, a, 0, 0, none, 0);
}
