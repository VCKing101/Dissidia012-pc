struct NodeSlot {
    void *value;
    NodeSlot *next;
};

struct NodeQueue {
    char pad_000[0x808];
    NodeSlot *first;
    NodeSlot *tail;

};

extern "C" {

extern void func_089EBCC8(void *node);

int func_089EADA4(NodeQueue *self, void *node)
{
    if (node == 0) {
        return -1;
    }
    func_089EBCC8(node);
    NodeSlot *tail = self->tail;
    for (NodeSlot *slot = self->first; slot->value != 0; slot = slot->next) {
    }
    tail->value = node;
    self->tail = self->tail->next;
    return 0;
}

}
