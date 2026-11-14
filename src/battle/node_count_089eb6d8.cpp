struct Node { Node *next; };
struct NodeList {
    char pad_000[0x810];
    Node *head;
    static Node *nextOf(NodeList *list, Node *node) { return node == 0 ? list->head : node->next; }
};
extern "C" {
extern int func_089EC188(Node *node);
extern int func_089EC2B0(Node *node);
extern int func_089EC260(Node *node);
extern int func_089EC288(Node *node);

/* Counts the nodes for which none of the four checks reports 1. */
int func_089EB6D8(NodeList *self)
{
    int count = 0;
    Node *node = 0;
    while ((node = NodeList::nextOf(self, node)) != 0) {
        if (func_089EC188(node) != 1 && func_089EC2B0(node) != 1 && func_089EC260(node) != 1 && func_089EC288(node) != 1) {
            count++;
        }
    }
    return count;
}
}
