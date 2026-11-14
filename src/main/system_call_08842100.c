typedef struct SystemRoot {
    int field_00;
    int field_04;
    int field_08;
} SystemRoot;

extern SystemRoot *D_089D4958;
extern void func_0885C6CC(int a, int b, int c, int d, int e, int f);

void func_08842100(int arg)
{
    int zero = 0;
    int one = 1;

    func_0885C6CC(D_089D4958->field_08, arg, 0, zero, one, 0);
}
