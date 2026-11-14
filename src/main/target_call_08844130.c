typedef struct CallTarget {
    int field_00;
    int field_04;
} CallTarget;

extern void func_0885C6CC(int a, int b, int c, int d, int e, int f);

void func_08844130(CallTarget *self, int arg)
{
    int zero = 0;

    func_0885C6CC(self->field_04, arg, 0, 0, zero, 0);
}
