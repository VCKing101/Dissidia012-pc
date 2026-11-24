/* Calls func_088CF970(D_089D7830, self->0x2C) and then with self->0x30. */
struct Self { char pad[0x2C]; int a; int b; };
extern "C" void *D_089D7830;
extern "C" void func_088CF970(void *owner, int h);
struct H { static void rel(int h) { func_088CF970(D_089D7830, h); } };
extern "C" void func_08B4E750(Self *self)
{
    H::rel(self->a);
    H::rel(self->b);
}
