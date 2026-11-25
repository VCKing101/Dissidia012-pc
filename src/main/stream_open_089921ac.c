typedef struct S { int a0; int h; int src; int dst; int p10; int w14; int w18; int w1c; float f20; float f24; float f28; int i2c; int p30; int p34; int sz38; int half3c; } S;
extern void func_08992724(S *self, int flag, int c);
extern int func_089C5CE0(int a, int b);
extern int func_089C5CF0(int a, int b);
extern void func_089C5CF8(int a, int *b);
extern void func_089C5D00(int a, int *b, int *c, int *d);
int func_089921AC(S *self, int a, int b, int c, int flagw)
{
    int flag = flagw & 0xFF;
    int h;
    int r;
    if (self->a0 >= 0) {
        return 0;
    }
    func_08992724(self, flag, b);
    self->src = a;
    self->dst = b;
    self->a0 = c;
    self->i2c = 0;
    self->f20 = 0.0f;
    self->f24 = 1.0f;
    self->f28 = 1.0f;
    h = self->h = func_089C5CE0(a, b);
    if (h < 0) {
        return 0;
    }
    func_089C5D00(h, &self->w14, &self->w18, &self->w1c);
    r = 0;
    if (self->w18 >= 0 && self->w1c >= 0) {
        r = func_089C5CF0(self->h, -1);
    }
    if (flag != 0) {
        func_089C5CF8(self->h, &self->sz38);
        self->half3c = self->sz38 / 2;
        if (self->half3c < 1) {
            self->half3c = 1;
        }
    }
    if (r < 0) {
        return 0;
    }
    return 1;
}
