/* Runs the per-frame checks and sends the update calls; the flag byte is set up front; -1 no argument, -2 no unit. */
typedef struct Obj38 { char pad[0x158]; void *h158; void *h15c; void *h160; } Obj38;
typedef struct Ref38 { int pad; void *unit; } Ref38;
extern Ref38 *func_08AB3264(void *h);
extern int func_08A8535C(void *u);
extern int func_088374B0(void *u);
extern int func_08A853CC(void *u);
extern void func_08A6A3F4(void *h, int a, unsigned char *flag, int b);
extern int func_08A65904(void *h);
extern void func_08A7444C(void *h, int a, int b, int c, int d, unsigned char *flag, int e, int f);
extern void func_08A9B8A8(Obj38 *self);
int func_08A9F9E4(Obj38 *self, unsigned char *flag)
{
    void *u;
    if (flag == 0) {
        return -1;
    } else {
        u = func_08AB3264(self->h160)->unit;
        if (u == 0) {
            return -2;
        } else {
            *flag = 1;
            if (func_08A8535C(u) == 1 || (func_088374B0(u) == 0 && func_08A853CC(u) == 1)) {
                func_08A6A3F4(self->h15c, 1, flag, 1);
            }
            if (*flag == 1 || func_08A65904(self->h158) == 1) {
                func_08A7444C(self->h15c, 0, 2, 3, 1, flag, 1, 0);
            }
            if (*flag == 0 && func_08A65904(self->h158) == 0) {
                func_08A9B8A8(self);
            }
            return 0;
        }
    }
}
