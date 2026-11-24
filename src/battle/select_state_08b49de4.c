/* Chooses the next state at +0x6E0: 4 when func_08B4A054 fails, else 3 after a started func_08B4A39C. */
struct Obj { int w0; char pad[0x2CC]; int w2d0; char pad2[0x40C]; int state; };
extern int func_08B4A054(struct Obj *);
extern int func_08B4A39C(struct Obj *, int, int);
extern void func_0892FF5C(void *, int, int);
void func_08B49DE4(struct Obj *self)
{
    if (func_08B4A054(self) == 0) {
        self->state = 4;
        return;
    }
    if (func_08B4A39C(self, 0, 1) != 0) {
        func_0892FF5C(&self->w2d0, self->w0, 2);
        self->state = 3;
    }
}
