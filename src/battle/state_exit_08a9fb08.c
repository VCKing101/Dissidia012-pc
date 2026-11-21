/* Leaves the state: runs the exit handlers when the check says so, then ends it; -1 when no argument. */
typedef struct Obj38 { char pad[0x148]; char sub[0x10]; void *h158; void *h15c; void *h160; } Obj38;
extern int func_08A7E904(void *sub, int id);
extern int func_08A65904(void *h);
extern void func_08A67C30(void *h);
extern void *func_08AB327C(void *h);
extern int func_08AB1B50(void *u, int id);
extern void *func_08AB3264(void *h);
extern void func_08A97910(void *u, int a, int b);
extern void func_08A9B8A8(Obj38 *self);
extern void func_08A7E858(void *sub, int id);
int func_08A9FB08(Obj38 *self, int arg)
{
    void *u;
    int two;
    if (arg == 0) {
        return -1;
    } else {
        two = 2;
        if (func_08A7E904(self->sub, two) == 0 || func_08A65904(self->h158) == 1) {
            func_08A67C30(self->h15c);
            u = func_08AB327C(self->h160);
            if (func_08AB1B50(u, 6) == 1) {
                u = func_08AB3264(self->h160);
                func_08A97910(u, 1, 5);
            }
            if (func_08A65904(self->h158) == 0) {
                func_08A9B8A8(self);
            }
        }
        func_08A7E858(self->sub, two);
        return 0;
    }
}
