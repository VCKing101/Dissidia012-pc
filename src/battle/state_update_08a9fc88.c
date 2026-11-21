/* Chooses between the two exit handlers from the unit state, clears the pending flag; -1 no argument, -2 no unit. */
typedef struct Obj38 { char pad[0xC]; char c; char pad2[0xE3]; unsigned char b0xf0; char pad3[0x66]; void *h158; void *h15c; void *h160; } Obj38;
typedef struct Ref38 { int pad; void *unit; } Ref38;
extern Ref38 *func_08AB3264(void *h);
extern int func_08A9C298(Obj38 *self);
extern int func_08AB344C(void *u);
extern int func_08AB3540(void *u);
extern void func_08A67CBC(void *h);
extern void func_08A68C54(void *h, int a);
extern int func_08A65904(void *h);
extern void func_08A9B82C(Obj38 *self);
extern void func_08A9B8A8(Obj38 *self);
int func_08A9FC88(Obj38 *self, int arg)
{
    void *u;
    void *h;
    if (arg == 0) {
        return -1;
    } else {
        u = func_08AB3264(self->h160)->unit;
        if (u == 0) {
            return -2;
        } else {
            if ((func_08A9C298(self) == 0 && func_08AB344C(u) == 1) || func_08AB3540(u) == 0) {
                func_08A67CBC(self->h15c);
                h = self->h158;
            } else {
                func_08A68C54(self->h15c, 0);
                h = self->h158;
            }
            if (func_08A65904(h) == 0) {
                func_08A9B82C(self);
                func_08A9B8A8(self);
            }
            self->b0xf0 = 0;
            return 0;
        }
    }
}
