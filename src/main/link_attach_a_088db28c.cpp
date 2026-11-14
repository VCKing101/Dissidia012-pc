/* Two link slots at 0xC/0x10: each holds either (id, wanted id) or the attached node. */
struct LinkNode {
    char pad_00[0x64];
    short id;
};

struct LinkKey {
    short id;
    short wanted;
};

struct LinkSlots {
    char pad_00[0xC];
    union {
        LinkKey key;
        LinkNode *node;
    } a;
    union {
        LinkKey key;
        LinkNode *node;
    } b;
    unsigned char flags;

    static void setId(LinkNode *node, short id)
    {
        node->id = id;
    }
};

extern "C" {
/* Attaches `node` in slot A when the wanted id matches; the slot's id moves to the node. */
void func_088DB28C(LinkSlots *slots, LinkNode *node, short id)
{
    if (slots->a.key.wanted == id) {
        LinkSlots::setId(node, slots->a.key.id);
        slots->a.node = node;
        slots->flags &= ~4;
    }
}
}
