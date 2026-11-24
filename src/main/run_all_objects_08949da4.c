// Marks self current, sets state 2, runs the per-object step on each object of the list at +0x68 (next at +0x154), then state 5.
struct N { char pad[0x154]; struct N *next; };
struct S { char pad[0x68]; struct N *head; char pad2[0x1C]; struct N *cur; char pad3[8]; unsigned char state; };
extern struct S *D_09B6F478;
extern void func_0894A5B0(struct S *, struct N *);
void func_08949DA4(struct S *self) {
    struct N *n;
    D_09B6F478 = self;
    self->state = 2;
    for (n = self->head; n != 0; n = n->next) {
        self->cur = n;
        func_0894A5B0(self, n);
    }
    self->state = 5;
}
