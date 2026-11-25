struct Core { char p[4]; int c4; char p8[0x28]; int c30; };
struct Obj {
    char p[0x50];
    int f50;
    float f54;
    int f58;
    char p5c[8];
    int f64;
};
extern "C" {
Core *func_08AB3264(int);
int func_08AB327C(int);
float func_08AB18C4(int);
float func_08AB1930(int);
int func_08AB199C(int);
float func_08838300(int);
int func_088377C0(int);
int func_08A84024(int);
int func_08A7479C(Obj *, int, int, int *, int);
int func_08A75D38(Obj *, int, int, int, int);

int func_08A6BDCC(Obj *self) {
    int c4, h;
    if (func_08AB3264(self->f64)->c30 == 2) return 0;
    c4 = func_08AB3264(self->f64)->c4;
    if (c4 == 0) return -1;
    h = func_08AB327C(self->f64);
    if (self->f50 == 0) {
        if (self->f54 <= 0.0f) self->f54 = func_08AB18C4(h);
        {
            float t = self->f54 - func_08838300(c4);
            self->f54 = t;
            if (t <= 0.0f) {
                if (func_08AB199C(h) == 1) self->f50 = 1;
                else self->f54 = func_08AB1930(h);
            }
        }
    }
    if (self->f50 == 1) {
        if (func_088377C0(c4) == 1) {
            self->f50 = 2;
        } else if (func_08A84024(c4) == 0) {
            int out;
            if (func_08A7479C(self, 0, 0, &out, 0) != 0) return -2;
            if (out >= 0) self->f58 = out;
        }
    }
    if (self->f50 == 2) {
        if (func_088377C0(c4) == 1) {
            if (self->f58 >= 0) { if (func_08A75D38(self, 1, 0, self->f58, 0) != 0) return -3; }
        } else {
            self->f50 = 0;
            self->f54 = 0.0f;
            self->f58 = -1;
        }
    }
    return 0;
}
}
