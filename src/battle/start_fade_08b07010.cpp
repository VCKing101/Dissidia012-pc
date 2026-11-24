// When the state at +0xB0 is 4, starts a fade (func_08873CE0 handle, two func_08873C20 keys 0 and 60.0), then moves to state 9; returns 0.
extern "C" {
extern int func_08873CE0(int, int, int);
extern void func_08873DEC(int, int);
extern void func_08873C20(float, int, int, int, int);
extern void func_08B081D4(void *, int);
}
struct State57 {
    char pad0[0xB0];
    int state;
    static bool is4(State57 *s) { return s->state == 4; }
};
extern "C" int func_08B07010(State57 *self)
{
    if (State57::is4(self) == 1) {
        int h = func_08873CE0(0, 0, 2);
        func_08873DEC(0, 2);
        func_08873C20(0.0f, h, 3, 0, 0);
        func_08873C20(60.0f, h, 2, 0, 0);
        func_08B081D4(self, 9);
    }
    return 0;
}
