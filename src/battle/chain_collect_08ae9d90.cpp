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

/* Count the nodes, copying up to max of them into out when given. */
int func_08AE9D90(ChainList *self, ChainNode **out, int max)
{
    ChainNode *node = 0;
    int count = 0;
    while ((node = ChainList::nextOf(self, node)) != 0) {
        if (out != 0) {
            if (count < max) {
                out[count] = node;
            } else {
                break;
            }
        }
        count++;
    }
    return count;
}

}
