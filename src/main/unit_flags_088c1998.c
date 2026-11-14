typedef struct Item { char data_00[0x1C]; unsigned int set; unsigned int clear; } Item;
typedef struct Equip { char data_00[0x30]; short items[2]; } Equip;
typedef struct Base { char data_00[0x18]; unsigned int flags; } Base;
typedef struct Unit { Base *base; Equip *equip; } Unit;
extern Item *func_088C7E98(int item);

unsigned int func_088C1998(Unit *u)
{
    unsigned int flags = 0;
    if (u->base != 0) {
        flags = u->base->flags;
    }
    if (u->equip != 0) {
        unsigned int clear = 0;
        unsigned int set = 0;
        int i;
        for (i = 0; i < 2; i++) {
            Item *item = func_088C7E98(*(short *)((char *)u->equip->items + i * 2));
            if (item != 0) {
                clear |= item->clear;
                set |= item->set;
            }
        }
        flags = (flags & ~clear) | set;
    }
    return flags;
}
