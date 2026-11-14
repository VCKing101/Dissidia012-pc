typedef struct System08889FB8 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System08889FB8;

extern System08889FB8 *D_089D6A10;
void func_089DDC98(void *player, int a, int b, unsigned char c);

void func_08889FB8(int a, int b, unsigned char c) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DDC98(D_089D6A10->player, a, b, c);
}
