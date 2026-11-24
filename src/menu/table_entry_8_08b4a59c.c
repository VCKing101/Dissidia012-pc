/* Returns the address of entry idx (8 bytes each, 0..0x45) in the table at +0x20 of **self, else 0. */
struct Tab { char pad[0x20]; char *entries; };
int func_08B4A59C(struct Tab **self, int idx)
{
    if (idx >= 0 && idx < 0x46) { return (int)((*self)->entries + idx * 8); } else { return 0; }
}
