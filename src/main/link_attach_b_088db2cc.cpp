/* Two link slots at 0xC/0x10: each holds either (id, wanted id) or the attached node. */
struct LinkNodeB {
    char pad_00[4];
    short id;
};

struct LinkKey {
    short id;
    short wanted;
};

struct LinkSlotsB {
    char pad_00[0xC];
    union {
        LinkKey key;
        void *node;
    } a;
    union {
        LinkKey key;
        LinkNodeB *node;
    } b;
    unsigned char flags;

    static void setId(LinkNodeB *node, short id)
    {
        node->id = id;
    }
};

extern "C" {
/* Attaches `node` in slot B when the wanted id matches; the slot's id moves to the node. */
void func_088DB2CC(LinkSlotsB *slots, LinkNodeB *node, short id)
{
    if (slots->b.key.wanted == id) {
        LinkSlotsB::setId(node, slots->b.key.id);
        slots->b.node = node;
        slots->flags &= ~8;
    }
}
}
