struct Item { char data_00[0x10]; short id; };
struct Equip { char data_00[0x30]; short items[2]; };
struct Base { int field_00; short id; };
struct Unit { Base *base; Equip *equip; };
extern "C" {
Item *func_088C7E98(int item);

int func_088C16DC(Unit *u, int slot)
{
    int id = -1;
    bool none = true;
    if (u->equip != 0) {
        Item *item = func_088C7E98(*(short *)((char *)u->equip->items + slot * 2));
        if (item != 0) {
            id = item->id;
        }
        none = id == -1;
    }
    if (none == true && u->base != 0 && slot == 0) {
        id = u->base->id;
    }
    return id;
}
}
