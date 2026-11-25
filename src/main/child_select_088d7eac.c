/* Selects variant `id` (0 or 1) of the owner's child: creates the child on first use, or switches it when its current variant differs. */
typedef struct Child { char p0[0xC]; short kind; } Child;
typedef struct S { char p0[4]; Child *child; int id; int val; int heap; } S;
extern int D_089CC180[];
extern int func_088D7EA4(S *self);
extern Child *func_088DAA40(int heap, int kind, int z);
extern void func_088D9F5C(Child *c, int kind, int flag);
int func_088D7EAC(S *self, unsigned int id, int val)
{
    int kind;
    if (func_088D7EA4(self) == 0) {
        return -1;
    }
    if (id >= 2) {
        return -2;
    }
    kind = D_089CC180[id];
    if (self->child == 0) {
        self->child = func_088DAA40(self->heap, kind, 0);
        if (self->child == 0) {
            return -3;
        }
    } else if (self->child->kind != kind) {
        func_088D9F5C(self->child, kind, 1);
    }
    self->id = id;
    self->val = val;
    return 0;
}
