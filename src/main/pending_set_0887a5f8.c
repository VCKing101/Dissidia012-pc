typedef struct Pending { char pad_00[0x14]; int a; int b; char pad_1C[0xC]; int count; } Pending;
int func_0887A5F8(Pending *self, int a, int b)
{
    int ok = 0;
    if (self->count < 2) {
        self->a = a;
        self->b = b;
        ok = 1;
    }
    return ok;
}
