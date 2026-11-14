typedef struct SlotNode {
    char pad_00[0x20];
    unsigned short flags;
} SlotNode;

typedef struct SlotWatch {
    char pad_00[0xC];
    int state;
    char pad_10[0x4];
    SlotNode *first[4];
    SlotNode *second[4];
} SlotWatch;

struct SlotNodeOps {
    static bool isDone(SlotNode *node) { return (node->flags & 0x20) != 0; }
};

extern "C" {

int func_089E87BC(SlotWatch *self)
{
    int all = 1;
    for (int i = 0; i < 4; i++) {
        if (self->first[i] != 0 && !SlotNodeOps::isDone(self->first[i])) {
            all = 0;
            break;
        }
        if (self->second[i] != 0 && !SlotNodeOps::isDone(self->second[i])) {
            all = 0;
            break;
        }
    }
    if (all == 1) {
        self->state = -1;
    }
    return 0;
}

}
