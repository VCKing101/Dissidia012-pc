typedef struct Node { char data_00[0x14]; unsigned char flags; char pad_15; unsigned char group; char data_17[0x24 - 0x17]; struct Node *next; } Node;
typedef struct Scene { char data_00[0x24]; Node *first; char data_28[0x30 - 0x28]; Node *second; } Scene;

void func_088CF6CC(Scene *s, unsigned char group)
{
    Node *n;
    Node *next;
    for (n = s->first; n != 0; n = next) {
        next = n->next;
        if (n->group == group || group == 0xFF) {
            n->flags &= ~2;
        }
    }
    for (n = s->second; n != 0; n = next) {
        next = n->next;
        if (n->group == group || group == 0xFF) {
            n->flags &= ~2;
        }
    }
}
