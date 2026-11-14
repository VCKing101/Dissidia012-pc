struct ListNode {
    ListNode *next;
};

struct NodeOwner {
    char pad_000[0x810];
    ListNode *head;
    char pad_814[0x8];
    char field_81C[0x4];

    static ListNode *nextOf(NodeOwner *owner, ListNode *node) { return node == 0 ? owner->head : node->next; }
};

extern "C" {

extern void func_089EB904(void *pool);
extern void func_089EBCC8(ListNode *node);
extern void func_089EA680(NodeOwner *self);

void func_089EA74C(NodeOwner *self)
{
    ListNode *node;
    func_089EB904(self->field_81C);
    node = 0;
    while ((node = NodeOwner::nextOf(self, node)) != 0) {
        func_089EBCC8(node);
    }
    func_089EA680(self);
}

}
