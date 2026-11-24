/* If the global check passes, sets the cursor up once and maps the owner's choice to a result code (default 2). */
extern int func_08B296A8(int g);
extern void func_08B296C8(int g);
extern void func_08B2A4CC(void *self);
extern int D_08B29CF8;

typedef struct P2A248 { int f0; int state; int choice; char pad[0xA0C - 0xC]; int f0A0C; } P2A248;
typedef struct S2A248 { char pad[0xA18]; P2A248 *p; } S2A248;

int func_08B2A248(S2A248 *self)
{
    P2A248 *p;
    int r;
    int one;
    if (func_08B296A8(D_08B29CF8) == 0) {
        return 0;
    }
    p = self->p;
    one = 1;
    if (p->f0A0C != one) {
        func_08B296C8(D_08B29CF8);
        p = self->p;
    }
    r = 2;
    if (p->choice == one) {
        func_08B2A4CC(self);
        self->p->state = 9;
        r = 0;
    } else if (p->choice == 2) {
        r = 3;
    } else if (p->choice == 3) {
        r = 1;
    }
    return r;
}
