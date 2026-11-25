/* Repositions both attached effects from slot data; true when the state is 4. */
struct O { int h0; int h1; char sub[8]; int r[2]; int state; };
extern "C" void func_088DA0A8(int, int, void *, void *);
extern "C" void func_0887B778(int, void *);
extern "C" void func_0887B820(int, int);
extern "C" int func_0891E78C(O *self)
{
    int tbl[2];
    int pad0[2];
    char buf[0x40];
    int loc;
    if (self->h1 != 0) {
        int one = 1;
        tbl[0] = 0;
        tbl[1] = one;
        for (int i = 0; i < 2; i++) {
            if (self->r[i] != 0) {
                int t = tbl[i];
                func_088DA0A8(self->h1, t, buf, &loc);
                func_0887B778(self->r[i], buf);
                func_0887B820(self->r[i], loc);
            }
        }
    }
    return self->state == 4;
}
