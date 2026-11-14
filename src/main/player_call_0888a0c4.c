typedef struct System0888A0C4 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A0C4;

extern System0888A0C4 *D_089D6A10;
void func_089DD9C0(void *player);

void func_0888A0C4(void) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DD9C0(D_089D6A10->player);
}
