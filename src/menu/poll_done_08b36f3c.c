/* Waits for the job handle at the +0xD0 object's +4 (func_088A751C); once finished or absent, clears it, runs func_08B469DC and sets the state word to 0x14. */
typedef struct J { int pad; int h; } J;
typedef struct C { int state; char pad[0xC]; int f10; } C;
typedef struct S { char pad[0xC4]; C *c; char pad2[0xD0 - 0xC8]; J *j; } S;
extern int func_088A751C(int h);
extern void func_08B469DC(int v);
void func_08B36F3C(S *self)
{
    if (self->j->h != 0) {
        if (func_088A751C(self->j->h)) return;
        self->j->h = 0;
    }
    func_08B469DC(self->c->f10);
    self->c->state = 0x14;
}
