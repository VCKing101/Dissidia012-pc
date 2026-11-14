typedef struct Handle {
    int res;
    int refs;
} Handle;

extern void func_0899BB1C(int res);

void func_08854AD0(Handle *self)
{
    int refs = self->refs - 1;
    self->refs = refs;
    if (refs > 0) {
        return;
    }
    func_0899BB1C(self->res);
    self->res = 0;
}
