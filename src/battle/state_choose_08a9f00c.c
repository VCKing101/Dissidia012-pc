/* Picks the next handler from the unit checks: marks flag 0xC or 0xD and starts, or runs the exit handlers; -1 no argument. */
typedef struct Obj38 { char pad[0xC]; unsigned char a; unsigned char b; char pad2[0x14A]; void *h158; void *h15c; void *h160; } Obj38;
extern void *func_08AB327C(void *h);
extern void *func_08AB3274(void *h);
extern int func_08A9B95C(Obj38 *self, int a);
extern int func_08AB1100(void *u);
extern int func_08A82638(void *u, int id);
extern void func_08A67AC0(void *h);
extern void func_08A6948C(void *h);
extern int func_08A65904(void *h);
extern void func_08A9B734(Obj38 *self);
extern void func_08A9B8A8(Obj38 *self);
int func_08A9F00C(Obj38 *self, int arg)
{
    void *u;
    void *v;
    int r;
    if (arg == 0) {
        return -1;
    } else {
        u = func_08AB327C(self->h160);
        v = func_08AB3274(self->h160);
        r = func_08A9B95C(self, 0);
        if (r >= 0 && func_08AB1100(u) == 1) {
            self->a = 1;
            func_08A67AC0(self->h15c);
        } else {
            if (func_08A82638(v, 0x18) == 1) {
                self->b = 1;
                func_08A67AC0(self->h15c);
            } else {
                func_08A6948C(self->h15c);
                if (func_08A65904(self->h158) == 0) {
                    func_08A9B734(self);
                    func_08A9B8A8(self);
                }
            }
        }
        return 0;
    }
}
