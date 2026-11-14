typedef struct System0888A224 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A224;

extern System0888A224 *D_089D6A10;
int func_089DD8DC(void *player);
void func_089ED20C(int stream, int value);

void func_0888A224(int value) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089ED20C(func_089DD8DC(D_089D6A10->player), value);
}
