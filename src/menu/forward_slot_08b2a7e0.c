/* Calls func_08B2E5D0(obj->0x708, &slot[obj->0x740] + 4) where slots are 0x48 bytes at work+0x3E758. */
struct Obj { char pad[0x708]; int a; char pad2[0x740 - 0x70C]; int idx; };
struct Self { char pad[0xA18]; struct Obj *obj; };
struct Slot { int a; char pad[0x44]; };
extern char *D_089D712C;
extern void func_08B2E5D0(int a, char *slot);
void func_08B2A7E0(struct Self *self)
{
    struct Obj *o = self->obj;
    struct Slot *base = (struct Slot *)(D_089D712C + 0x3E758);
    func_08B2E5D0(o->a, (char *)(base + o->idx) + 4);
}
