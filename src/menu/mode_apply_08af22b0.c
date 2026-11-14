extern int D_08B00B1C[];
extern void func_08AF7C74(void *self);
extern void func_08AF7AEC(void *self);
extern void func_08AF7A38(void *self);

void func_08AF22B0(void *self, int kind)
{
    func_08AF7C74(self);
    if (D_08B00B1C[kind] == 1) {
        func_08AF7AEC(self);
    } else {
        func_08AF7A38(self);
    }
}
