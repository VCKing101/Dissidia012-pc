// Installs two hook functions, falling back to the defaults when either is null.
extern "C" void func_08940424(void);
extern "C" void func_0894042C(void);
extern "C" void *D_089D9140[];
extern "C" void *D_089D914C[];
struct Hooks {
    static void Set(void *a, void *b) {
        void *x = a ? a : (void *)func_08940424;
        void *y = b ? b : (void *)func_0894042C;
        D_089D9140[2] = x; D_089D914C[2] = y;
    }
};
extern "C" void func_089403D8(void *a, void *b) { Hooks::Set(a, b); }
