/* Returns 0 when self->0x30 is set and its halfword at 0xC is 0x19, else 1. */
struct Sub { char pad[0xC]; short kind; };
struct Self { char pad[0x30]; struct Sub *sub; };
int func_08B4C218(struct Self *self)
{
    if (self->sub != 0) { if (self->sub->kind == 0x19) { return 0; } }
    return 1;
}
