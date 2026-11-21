/* Handles the state change: starts the transition when the check passes, else runs the exit handlers; -1 no argument. */
typedef struct Obj38 { char pad[0xC]; unsigned char a; unsigned char b; char pad2[0x14A]; void *h158; void *h15c; void *h160; } Obj38;
typedef struct Ref38 { char pad[0x20]; int value; } Ref38;
extern int func_08A9BC74(Obj38 *self, char *buf);
extern void *func_08AB327C(void *h);
extern int func_08AB11A4(void *u);
extern void func_08A68C54(void *h, int a);
extern int func_08A65904(void *h);
extern void func_08A9B7B0(Obj38 *self);
extern void func_08A9B8A8(Obj38 *self);
extern Ref38 *func_08AB3264(void *h);
extern void func_08A73864(void *h, int a, int b, int c, int d, int e);
int func_08A9F454(Obj38 *self, int arg)
{
    char buf[16];
    int r;
    void *u;
    if (arg == 0) {
        return -1;
    } else {
        r = func_08A9BC74(self, buf);
        u = func_08AB327C(self->h160);
        if (r >= 0 && (self->a == 1 || func_08AB11A4(u) == 1)) {
            func_08AB3264(self->h160)->value = r;
            func_08A73864(self->h15c, 0, 0, 0, 0, 0);
        } else {
            func_08A68C54(self->h15c, 0);
            if (func_08A65904(self->h158) == 0) {
                func_08A9B7B0(self);
                func_08A9B8A8(self);
            }
        }
        self->a = 0;
        self->b = 0;
        return 0;
    }
}
