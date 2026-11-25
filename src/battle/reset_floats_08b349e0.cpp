// Zeroes two float pairs for two players, then initialises the matrix at +0x40 and sets the float at +0x10F0 to 1.
extern "C" void func_089A3EBC(void *m);
struct S08B349E0 {
    char pad0[0x40];
    char m[0x10A0];
    float v[4];
    float one;
};
extern "C" void func_08B349E0(S08B349E0 *self)
{
    for (int i = 0; i < 2; i++) {
        float zero = 0.0f;
        self->v[i] = zero;
        self->v[i + 2] = zero;
    }
    self->one = 1.0f;
    func_089A3EBC(self->m);
}
