typedef struct FindNode {
    int field_00;
    int id;
} FindNode;

typedef struct NodeOwner {
    unsigned char active;
} NodeOwner;

extern FindNode *func_08A65D38(NodeOwner *self, FindNode *node);

int func_08A6587C(NodeOwner *self, int id)
{
    int result;
    FindNode *node;
    if (!self->active) {
        return 0;
    }
    result = 0;
    node = 0;
    while ((node = func_08A65D38(self, node)) != 0) {
        if (node->id == id) {
            result = 1;
            break;
        }
    }
    return result;
}
