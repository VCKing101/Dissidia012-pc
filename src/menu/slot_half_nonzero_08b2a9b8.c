/* Returns whether the halfword at +6 of the 0x48-byte slot (index obj->0x740) at work+0x3E758 is nonzero. */
struct Obj { char pad[0x740]; int idx; };
struct Self { char pad[0xA18]; struct Obj *obj; };
struct Slot { char pad[6]; short h; char pad2[0x40]; };
extern char *D_089D712C;
int func_08B2A9B8(struct Self *self)
{
    struct Slot *s = (struct Slot *)(D_089D712C + 0x3E758) + self->obj->idx;
    if (s->h == 0) { return 0; } else { return 1; }
}
