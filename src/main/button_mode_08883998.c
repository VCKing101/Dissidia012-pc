extern unsigned char D_089D69DF;
void func_0888337C(unsigned char on);

int func_08883998(int unused, unsigned int buttons) {
    if (buttons & 0x90000) {
        func_0888337C(1);
        D_089D69DF = 1;
    } else if (buttons & 0x40000) {
        D_089D69DF = 0;
    }
    return 0;
}
