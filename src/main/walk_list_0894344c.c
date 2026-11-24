// Walks the linked list at +0x5C (next at +0x20), running the per-node step with the owner and node.
struct N { char pad[0x20]; struct N *next; };
struct S { char pad[0x5C]; struct N *head; };
extern void func_089432B4(struct S *, struct N *);
void func_0894344C(struct S *self) {
    struct N *n = self->head;
    while (n != 0) {
        struct N *next = n->next;
        func_089432B4(self, n);
        n = next;
    }
}
