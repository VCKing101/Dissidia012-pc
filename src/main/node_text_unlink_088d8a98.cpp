/* Unlink a layout node's label text from the node's text list (text +0x4C next / +0x54 prev, node +0x30 head,
   +0x34 tail, +0x38 count), clear its node-owned flag 0x8000, then close the label handle (0x0887A354).
   See docs/decomp/text-system.md, section 4 (layout nodes). */
struct NodeText {
    char pad0[0x10];
    unsigned int flags;
    char pad1[0x38];
    NodeText *next;
    char pad2[4];
    NodeText *prev;
};
struct LayoutNode {
    char pad0[0x30];
    NodeText *head;
    NodeText *tail;
    int count;
};
struct NodeLabel {
    char pad0[4];
    LayoutNode *list;
    char handle[8];
};
extern "C" NodeText *func_0887A4C4(void *h);
extern "C" void func_0887A354(void *h);
struct NodeTextList {
    static void unlink(NodeLabel *self, NodeText *it) {
        LayoutNode *list = self->list;
        if (it != 0) {
            it->flags &= 0xFFFF7FFF;
            if (it->prev == 0) {
                list->head = it->next;
            } else {
                NodeText **s = &it->prev->next;
                *s = it->next;
            }
            NodeText *p = it->prev;
            NodeText *n = it->next;
            NodeText **t;
            if (n == 0) {
                list->tail = p;
            } else {
                t = &n->prev;
                *t = p;
            }
            it->prev = 0;
            it->next = 0;
            list->count--;
        }
    }
};
extern "C" void func_088D8A98(NodeLabel *self)
{
    NodeText *it = func_0887A4C4(self->handle);
    if (self->list != 0 && it != 0) {
        NodeTextList::unlink(self, it);
    }
    func_0887A354(self->handle);
}
