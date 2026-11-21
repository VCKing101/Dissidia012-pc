/* Decides whether the unit should react: skips on the listed conditions, else may trigger a handler call; -1 when no unit. */
typedef struct Obj38 { char pad[0x158]; void *h158; void *h15c; void *h160; void *h164; } Obj38;
typedef struct Ref38 { int pad; void *unit; int pad2[10]; int state; } Ref38;
extern Ref38 *func_08AB3264(void *h);
extern int func_08837458(void *u);
extern int func_08837310(void *u);
extern int func_08A6587C(void *h, int id);
extern int func_08A85668();
extern int func_08B337A0(void *u);
extern int func_0883923C(void *u);
extern void *func_08AB327C(void *h);
extern int func_08AAF6A0(void *u);
extern void func_08A97060(void *h, int id);
int func_08A9B0AC(Obj38 *self, unsigned char mode)
{
    void *unit;
    void *u;
    unit = func_08AB3264(self->h160)->unit;
    if (unit == 0) {
        return -1;
    } else if (mode == 0 && (func_08837458(unit) == 1 || func_08837310(unit) == 1 || func_08A6587C(self->h158, 0xC) == 1)) {
        return 0;
    } else {
        if (func_08A85668() == 1) {
            return 0;
        } else {
            if (func_08B337A0(unit) == 1 && func_0883923C(unit) == 0) {
                u = func_08AB327C(self->h160);
                if (func_08AB3264(self->h160)->state != 2 && func_08AAF6A0(u) == 1) {
                    func_08A97060(self->h164, 0x12);
                }
            }
            return 0;
        }
    }
}
