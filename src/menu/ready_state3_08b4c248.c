/* Returns 1 when self->0x28 is 3 and self->0x54 is at least 0x3C. */
struct Self { char pad[0x28]; int state; char pad2[0x54 - 0x2C]; int timer; };
int func_08B4C248(struct Self *self)
{
    if (self->state == 3 && self->timer >= 0x3C) { return 1; } else { return 0; }
}
