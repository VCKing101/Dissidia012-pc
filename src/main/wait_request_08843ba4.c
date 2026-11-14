typedef struct Waiter08843BA4 {
    unsigned char unknown_00[0x58];
    int count;
    unsigned char unknown_5C[4];
    unsigned char state;
    unsigned char done;
} Waiter08843BA4;

extern Waiter08843BA4 *D_089D4960;

void func_08843BA4(int count) {
    if (count <= 0) {
        D_089D4960->state = 0xB;
        D_089D4960->done = 1;
    } else {
        D_089D4960->state = 10;
        D_089D4960->count = count;
    }
}
