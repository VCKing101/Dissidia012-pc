typedef struct Slot08873F10 {
    unsigned char unknown_000[0x234];
    int active;
    unsigned char unknown_238[8];
} Slot08873F10;

extern "C" Slot08873F10 D_09B47B50[3];

extern "C" int func_08873F10(void) {
    for (int i = 0; i < 3; i++) {
        if (D_09B47B50[i].active != 0) {
            return 1;
        }
    }
    return 0;
}
