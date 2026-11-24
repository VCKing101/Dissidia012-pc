/* Returns whether the word at +0x28 differs from 2. */
struct Self { char pad[0x28]; int state; };
int func_08B530A0(struct Self *self)
{
    int two = 2;
    if (self->state == two) { return 0; } else { return 1; }
}
