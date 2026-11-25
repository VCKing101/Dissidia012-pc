/* Sizes the three widgets to a cell count (clamped to the first widget's extent), clears their bit 1 and refreshes with count + 1. */
typedef struct W267F4 { char pad[0x20]; unsigned short fl; } W267F4;
typedef struct D267F4 { char pad0[8]; W267F4 *b; W267F4 *c; char pad1[0x1C - 0x10]; W267F4 *a; char pad2[0x1C8 - 0x20]; float step; } D267F4;
typedef struct S267F4 { D267F4 *d; } S267F4;
extern int func_088DA024(W267F4 *);
extern void func_088D9AEC(W267F4 *, float);
extern void func_08B265A8(S267F4 *, int);
void func_08B267F4(S267F4 *self, int count)
{
    int cell[2];
    int mask;
    cell[0] = func_088DA024(self->d->a);
    cell[1] = (int)(self->d->step * (float)count);
    if (!(cell[1] < cell[0])) cell[1] = cell[0];
    func_088D9AEC(self->d->a, (float)cell[1]);
    mask = -3;
    self->d->a->fl &= mask;
    func_088D9AEC(self->d->b, (float)cell[1]);
    self->d->b->fl &= mask;
    func_088D9AEC(self->d->c, (float)cell[1]);
    self->d->c->fl &= mask;
    func_08B265A8(self, count + 1);
}
