typedef union Mode08897924 {
    unsigned int word;
    struct {
        unsigned int low : 8;
        unsigned int blend : 2;
        unsigned int unknown_10 : 4;
        unsigned int enabled : 1;
        unsigned int high : 17;
    } bits;
} Mode08897924;

void func_0889C708(int a, int b, int c, unsigned int mode, unsigned int alt, int d, int e, int f);

void func_08897924(int a, int b, int c, int d, int e, int f) {
    Mode08897924 mode;
    Mode08897924 alt;
    mode.word = 0;
    mode.bits.enabled = 1;
    mode.bits.blend = 3;
    alt.word = 0;
    alt.bits.enabled = 1;
    alt.bits.blend = 1;
    func_0889C708(a, b, c, mode.word, alt.word, d, e, f);
}
