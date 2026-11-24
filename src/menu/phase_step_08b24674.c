/* Clears the busy flag, sets it again when the check passes (unless flag 0x10 is set), then runs the current phase handler. */
extern int func_08B249AC(void *self);
extern void func_08B24850(void *self);
extern void func_08B2473C(void *self);
typedef struct P24674 { char pad[0x910]; unsigned flags; int phase; } P24674;
typedef struct S24674 { char pad[0x13C]; P24674 *p; } S24674;
void func_08B24674(S24674 *self)
{
    P24674 *p;
    self->p->flags = self->p->flags & 0xFFF7FFFF;
    p = self->p;
    if ((p->flags & 0x10) == 0) {
        int r = func_08B249AC(self);
        p = self->p;
        if (r != 0) {
            p->flags = p->flags | 0x80000;
            p = self->p;
        }
    }
    switch (p->phase) {
    case 0:
        p->phase = 100;
        break;
    case 100:
        func_08B2473C(self);
        break;
    case 200:
        func_08B24850(self);
        break;
    case 10000:
        break;
    default:
        break;
    }
}
