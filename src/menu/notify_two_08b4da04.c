/* With i = self->0x90: if a is set calls func_088D9CD8(self->slot[i]), if b is set calls func_088D9CD8(self->slot[i + 5]); slots start at +0x10. */
struct Self { char pad[0x10]; int slot[10]; char pad2[0x90 - 0x38]; unsigned idx; };
extern void func_088D9CD8(int h);
void func_08B4DA04(struct Self *self, unsigned char a, unsigned char b)
{
    int fa = a;
    int fb = b;
    unsigned i = self->idx;
    if (fa != 0) { func_088D9CD8(self->slot[i]); }
    if (fb != 0) { func_088D9CD8(self->slot[i + 5]); }
}
