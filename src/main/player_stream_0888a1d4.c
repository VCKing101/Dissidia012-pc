typedef struct System0888A1D4 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A1D4;

extern System0888A1D4 *D_089D6A10;
int func_089DD8DC(void *player);
void func_089ED1E4(int stream, int value);

void func_0888A1D4(int value) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089ED1E4(func_089DD8DC(D_089D6A10->player), value);
}
