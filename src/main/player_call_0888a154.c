typedef struct System0888A154 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888A154;

extern System0888A154 *D_089D6A10;
void func_089DDB8C(void *player, int a, int b);

void func_0888A154(int a, int b) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DDB8C(D_089D6A10->player, a, b);
}
