struct Node { Node *next; };
struct NodeList {
    char pad_000[0x810];
    Node *head;
    static Node *nextOf(NodeList *list, Node *node) { return node == 0 ? list->head : node->next; }
};
extern "C" {
extern int func_089E9C64(void);
extern int func_089E9B18(void);
extern int func_089EC338(Node *node);
extern int func_089EC188(Node *node);
extern int func_089EC2B0(Node *node);
extern void func_089EC114(Node *node);

/* Releases every node that is finished (or, in mode 1, idle on both checks). */
int func_089EAEEC(NodeList *self)
{
    if (func_089E9C64() == 1) {
        int mode = func_089E9B18();
        Node *node = self->head;
        while (node != 0) {
            Node *next = NodeList::nextOf(self, node);
            if (func_089EC338(node) == 0 || (mode == 1 && func_089EC188(node) == 0 && func_089EC2B0(node) == 0)) {
                func_089EC114(node);
            }
            node = next;
        }
    }
    return 0;
}
}
