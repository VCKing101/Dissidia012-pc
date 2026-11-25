/* Marks self as pending (byte 0x45 = 1), stores three words at +0x48..+0x50 and returns 1. */
/* Marks self as pending (byte 0x45 = 1) and stores three words at +0x48, +0x4C, +0x50. */
struct Self { char pad[0x45]; unsigned char pending; char pad2[2]; int a, b, c; };
int func_08B52880(struct Self *self, int a, int b, int c)
{
    int one = 1;
    self->pending = one;
    self->a = a;
    self->b = b;
    self->c = c;
    return one;
}
