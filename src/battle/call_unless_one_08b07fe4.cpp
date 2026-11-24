// Calls func_08B081D4(self, 0x11) unless the word at +0x88 equals 1; returns 0.
struct Obj08B07FE4 {
    char pad[0x88];
    int w88;
    static bool Eq(int a, int b) { return a == b; }
};
extern "C" {
extern void func_08B081D4(void *, int);
int func_08B07FE4(Obj08B07FE4 *self)
{
    if (!Obj08B07FE4::Eq(self->w88, 1)) {
        func_08B081D4(self, 0x11);
    }
    return 0;
}
}
