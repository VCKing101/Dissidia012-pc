// When the object exists: clears the screen tint, resets the effect globals, closes its sub-object and optionally frees it.
extern "C" {
extern void func_0887511C(void *obj, float *col, int a, int b);
extern void func_08A620B0(int g, int a);
extern void func_08B114A0(int h, int a);
extern void func_08890970(void *p);
extern void *D_089D6028;
extern int D_08A62280;
}
struct Obj08B10A6C {
    char pad[0x30];
    int h;
    static void run(Obj08B10A6C *self, unsigned flags)
    {
        float col[4];
        if (self != 0) {
            void *g = D_089D6028;
            col[0] = 0.0f;
            col[1] = 0.0f;
            col[2] = 0.0f;
            col[3] = 0.0f;
            func_0887511C(g, col, 0, 0);
            func_08A620B0(D_08A62280, 0);
            func_08B114A0(self->h, 3);
            if ((flags & 1) != 0 && self != 0) {
                func_08890970(self);
            }
        }
    }
};
extern "C" void func_08B10A6C(Obj08B10A6C *self, unsigned flags) { Obj08B10A6C::run(self, flags); }
