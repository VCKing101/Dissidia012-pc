// Installs two hook functions, falling back to the defaults when either is null.
extern "C" void func_08934AF4(void);
extern "C" void func_08934B18(void);
extern "C" void *D_089D9140[];
extern "C" void *D_089D914C[];
struct Hooks {
    static void Set(void *a, void *b) {
        void *x = a ? a : (void *)func_08934AF4;
        void *y = b ? b : (void *)func_08934B18;
        D_089D9140[1] = x; D_089D914C[1] = y;
    }
};
extern "C" void func_089403A0(void *a, void *b) { Hooks::Set(a, b); }
