typedef struct System08889F70 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System08889F70;

extern System08889F70 *D_089D6A10;
void func_089DDC10(void *player, int a, int b);

void func_08889F70(int a, int b) {
    if (D_089D6A10->player == 0) {
        return;
    }
    func_089DDC10(D_089D6A10->player, a, b);
}
