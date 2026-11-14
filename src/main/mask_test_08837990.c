typedef struct Actor08837990 {
    unsigned char unknown_000[0x388];
    unsigned long long mask;
} Actor08837990;

extern unsigned long long D_089C7FC0;

int func_08837990(Actor08837990 *self) {
    return (self->mask & D_089C7FC0) != 0;
}
