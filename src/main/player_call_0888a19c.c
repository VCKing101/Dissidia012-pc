typedef struct System0888A19C {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A19C;

extern System0888A19C *D_089D6A10;
void func_089DDA60(void *player);

void func_0888A19C(void) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DDA60(D_089D6A10->player);
}
