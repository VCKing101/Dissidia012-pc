typedef struct Actor08834D8C {
    unsigned char unknown_000[0x388];
    unsigned long long mask;
} Actor08834D8C;

extern unsigned long long D_089C7F80;

void func_08834D30(Actor08834D8C *self, int arg, float a, float b);

void func_08834D8C(Actor08834D8C *self, int arg, float a) {
    func_08834D30(self, arg, a, 0.0f);
    self->mask |= D_089C7F80;
}
