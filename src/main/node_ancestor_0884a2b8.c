/* Walks up from a node to the first ancestor at or below the level. */
typedef struct Node0884A2B8 {
    char pad_00[8];
    short level;
    short parent;
} Node0884A2B8;

Node0884A2B8 *func_0884808C(void *tree, int id);

Node0884A2B8 *func_0884A2B8(void *tree, Node0884A2B8 *node, int level)
{
    if (node->parent < 0) {
        return 0;
    }
    while (node->level < level) {
        if (node->parent < 0) {
            break;
        }
        node = func_0884808C(tree, node->parent);
    }
    return node;
}
