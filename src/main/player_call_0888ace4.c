typedef struct System0888ACE4 {
    unsigned char unknown_000[0x7A4];
    void *player;
} System0888ACE4;

extern System0888ACE4 *D_089D6A10;
int func_089DDD74(void *player, int a);

int func_0888ACE4(int a) {
    if (D_089D6A10->player == 0) {
        return -1;
    }
    return func_089DDD74(D_089D6A10->player, a);
}
