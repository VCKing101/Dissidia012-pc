extern "C" int D_089D5FE4;

struct Pow208871248 {
    static float ratio() {
        int p = 1;
        int n = D_089D5FE4;
        while (p <= n) {
            p <<= 1;
        }
        return (float)n / (float)p;
    }
};

extern "C" {
float func_08871248(void) {
    return Pow208871248::ratio();
}
}
