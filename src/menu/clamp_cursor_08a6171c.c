/* Picks a start index for the current page kind, clamps it to that kind's limit, applies it and sets flag bit 0. */
typedef struct Cc { char pad0[0xD0]; int kind; char pad1[0xF8 - 0xD4]; unsigned flags; int cur; } Cc;
extern const int D_08A61DD0[];
extern void func_088D46E4(void *, int, int, int, int, int);
void func_08A6171C(Cc *self) {
    int kind = self->kind;
    int idx = self->cur;
    int limit, lim, over;
    switch (kind) {
    case 0:
        idx = idx > 1 ? 3 : 0;
        break;
    case 1:
        idx = idx > 2 ? 2 : 0;
        break;
    case 2:
        idx = 0;
        break;
    case 3:
        idx = 0;
        break;
    }
    lim = D_08A61DD0[kind];
    over = idx < lim;
    limit = lim - 1;
    if (!over) idx = limit;
    func_088D46E4(&self->flags, idx, 0, limit, 0, -1);
    self->flags |= 1;
}
