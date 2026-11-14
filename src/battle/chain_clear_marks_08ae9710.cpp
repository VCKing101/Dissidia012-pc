typedef struct ChainNode {
    char pad_00[0x28];
    union {
        unsigned int word;
        struct {
            unsigned int bit0 : 1;
            unsigned int bit1 : 1;
            unsigned int marked : 1;
            unsigned int selected : 1;
        } bits;
    } flags;
    struct ChainNode *next;
} ChainNode;

typedef struct ChainList {
    ChainNode *head;
    char pad_04[0x1FC];
    int count;
    unsigned char flag_204;
    unsigned char flag_205;

    static ChainNode *nextOf(ChainList *list, ChainNode *node) { return node == 0 ? list->head : node->next; }
} ChainList;

extern "C" {

/* Clear the mark and selection of every node. */
void func_08AE9710(ChainList *self)
{
    ChainNode *node = 0;
    self->count = 0;
    while ((node = ChainList::nextOf(self, node)) != 0) {
        node->flags.bits.marked = 0;
        node->flags.bits.selected = 0;
    }
    self->flag_204 = 0;
    self->flag_205 = 0;
}

}
