/* Clears byte 0x44 of the linked object (if any), then calls func_08B4F3E0. */
struct Link { char pad[0x44]; unsigned char flag; };
struct Obj { char pad[0x1C]; struct Link *link; };
extern void func_08B4F3E0(struct Obj *);
void func_08B4F738(struct Obj *self)
{
    if (self->link != 0) {
        int z = 0; self->link->flag = z;
    }
    func_08B4F3E0(self);
}
