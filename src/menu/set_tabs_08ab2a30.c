/* For each of the two tabs resets the tab object's counters and, depending on whether the tab is the selected one (+0x540), poses it (6 or 5) and registers its text twice or once with func_088D40BC; then runs func_08AB2B5C. */
typedef struct Tab08AB2A30 { char pad[0x30]; int w30; int w34; int w38; } Tab08AB2A30;
typedef struct Blk08AB2A30 {
    char pad0[0x10];
    char at10[0x480];
    char at490[8];
    int h498[2];
    char pad1[0x540 - 0x4A0];
    int sel;
} Blk08AB2A30;
typedef struct S08AB2A30 { char pad[0x584]; Blk08AB2A30 *blk; } S08AB2A30;
extern void func_088D9F5C(int h, int a, int b);
extern void func_088D40BC(char *a, char *b, int c, int d, int e, int f);
extern void func_0887B944(int h);
extern void func_08AB2B5C(S08AB2A30 *self);
void func_08AB2A30(S08AB2A30 *self)
{
    int i;
    for (i = 0; i < 2; i++) {
        int next;
        Tab08AB2A30 *t = ((Tab08AB2A30 **)((char *)self->blk + 0x1C))[i];
        t->w34 = 0;
        t->w30 = 0;
        t->w38 = 0;
        next = (i + 3) & 0xFFFF;
        if (self->blk->sel == i) {
            func_088D9F5C(((int *)((char *)self->blk + 0x1C))[i], 6, 1);
            func_088D40BC(self->blk->at10, self->blk->at490, (i + 2) & 0xFFFF, 0x12, next, 0);
            func_088D40BC(self->blk->at10, self->blk->at490, i & 0xFFFF, 0x12, next, 1);
        } else {
            func_088D9F5C(((int *)((char *)self->blk + 0x1C))[i], 5, 1);
            func_0887B944(self->blk->h498[i]);
            func_088D40BC(self->blk->at10, self->blk->at490, i & 0xFFFF, 0x12, next, 1);
        }
    }
    func_08AB2B5C(self);
}
