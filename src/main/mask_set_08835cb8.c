typedef struct Actor08835CB8 {
    unsigned char unknown_000[0x40];
    unsigned char field_040[0x348];
    unsigned long long mask;
} Actor08835CB8;

extern unsigned long long D_089C7F98;

void func_089A3EFC(void *p, int arg);

void func_08835CB8(Actor08835CB8 *self, int arg) {
    self->mask |= D_089C7F98;
    func_089A3EFC(self->field_040, arg);
}
