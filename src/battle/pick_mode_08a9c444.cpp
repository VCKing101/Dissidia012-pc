/* Picks the animation mode (0..3) from the unit's flags and the state; 0 when there is no unit. */
struct Obj38 { char pad[0x158]; void *h158; void *h15c; void *h160; };
struct Unit38;
struct Ref38 { int pad; Unit38 *unit; int pad2[10]; int state; };
struct Sub38 { char pad[8]; unsigned char flags; static unsigned char Has2(Sub38 *s) { return (s->flags & 2) != 0 ? 1 : 0; } };
struct Unit38 { char pad[0x190]; Sub38 sub; };
extern "C" Ref38 *func_08AB3264(void *h);
extern "C" int func_08A6587C(void *h, int id);
extern "C" void *func_08AB3274(void *h);
extern "C" int func_08A82638(void *u, int id);
extern "C" int func_08A66424(void *h);
extern "C" void *func_08AB3294(void *h);
extern "C" int func_08A7ED68(void *u);
extern "C" int func_08A9C444(Obj38 *self)
{
    Unit38 *unit;
    int a;
    int b;
    int state;
    int kind;
    int c;
    void *u;
    Sub38 *sub;
    unit = func_08AB3264(self->h160)->unit;
    if (unit == 0) {
        return 0;
    } else {
        a = func_08A6587C(self->h158, 4);
        u = func_08AB3274(self->h160);
        if (func_08A82638(u, 0x11) == 1) {
            a = 1;
        }
        sub = &unit->sub;
        if (Sub38::Has2(sub) == 1) {
            a = 1;
        }
        if (func_08A66424(self->h15c) == 1) {
            a = 1;
        }
        state = func_08AB3264(self->h160)->state;
        u = func_08AB3294(self->h160);
        kind = func_08A7ED68(u);
        c = func_08A6587C(self->h158, 1);
        if (a == 1 || (state == 2 && kind == 3)) {
            return 2;
        } else if (c == 1 || (state == 2 && (kind == 8 || kind == 0))) {
            return 1;
        } else {
            if (func_08A6587C(self->h158, 0xB) == 1) {
                return 3;
            } else {
                return 0;
            }
        }
    }
}
