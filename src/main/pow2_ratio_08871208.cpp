extern "C" int D_089D5FE0;

struct Pow208871208 {
    static float ratio() {
        int p = 1;
        int n = D_089D5FE0;
        while (p <= n) {
            p <<= 1;
        }
        return (float)n / (float)p;
    }
};

extern "C" {
float func_08871208(void) {
    return Pow208871208::ratio();
}
}
