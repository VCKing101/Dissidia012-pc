// Sets both entries of the two hook tables, defaulting each to its built-in handler when none is given.
extern "C" void func_08934AF4(void);
extern "C" void func_08934B18(void);
extern "C" void *D_089D9140[];
extern "C" void *D_089D914C[];
struct H {
    static void run(void *a, void *b) {
        void *x = a ? a : (void *)func_08934AF4;
        void *y = b ? b : (void *)func_08934B18;
        D_089D9140[0] = x; D_089D914C[0] = y; D_089D9140[1] = x; D_089D914C[1] = y;
    }
};
extern "C" void func_08940360(void *a, void *b) { H::run(a, b); }
