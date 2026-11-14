typedef struct System0888A274 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A274;

extern System0888A274 *D_089D6A10;
int func_089DD8DC(void *player);
void func_089ED23C(int stream);

void func_0888A274(void) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089ED23C(func_089DD8DC(D_089D6A10->player));
}
