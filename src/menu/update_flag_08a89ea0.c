/* When the mode byte is not 1 and flag bits 0x2000 and 0x4000 are set: sets or clears bit 0x8000 from the manager query; returns the query result (0 otherwise). */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern int func_088FDC98(int g);
int func_08A89EA0(int self) {
    int r;
    unsigned w;
    if (D_089D5F20 == 1) {
        return 0;
    }
    w = *(unsigned *)(*(int *)(self + 0x30C) + 0x41C);
    if ((w & 0x2000) == 0) {
        return 0;
    }
    if ((w & 0x4000) == 0) {
        return 0;
    }
    r = func_088FDC98(D_089D7F90);
    w = *(unsigned *)(*(int *)(self + 0x30C) + 0x41C);
    if (r == 1) {
        w = w | 0x8000;
    } else {
        w = w & 0xFFFF7FFF;
    }
    *(unsigned *)(*(int *)(self + 0x30C) + 0x41C) = w;
    return r;
}
