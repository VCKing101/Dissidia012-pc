typedef struct Waiter08843B2C {
    unsigned char unknown_00[0x58];
    int count;
    unsigned char unknown_5C[4];
    unsigned char state;
    unsigned char done;
} Waiter08843B2C;

extern Waiter08843B2C *D_089D4960;

void func_08843B2C(int count) {
    if (count <= 0) {
        D_089D4960->state = 0xB;
        D_089D4960->done = 1;
    } else {
        D_089D4960->state = 6;
        D_089D4960->count = count;
    }
}
