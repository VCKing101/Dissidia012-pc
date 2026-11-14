typedef struct System0888A0FC {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A0FC;

extern System0888A0FC *D_089D6A10;
void func_089DDA10(void *player);

void func_0888A0FC(void) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DDA10(D_089D6A10->player);
}
