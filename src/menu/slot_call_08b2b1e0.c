/* Calls func_0892A6B4(&slot[obj->0x740] at work+0x3E75C, obj + 0x770). */
struct Obj { char pad[0x740]; int idx; };
struct Self { char pad[0xA18]; struct Obj *obj; };
struct Slot { int a; char pad[0x44]; };
extern char *D_089D712C;
extern void func_0892A6B4(char *slot, char *arg);
void func_08B2B1E0(struct Self *self)
{
    struct Obj *o = self->obj;
    struct Slot *base = (struct Slot *)(D_089D712C + 0x3E758);
    func_0892A6B4((char *)(base + o->idx) + 4, (char *)o + 0x770);
}
