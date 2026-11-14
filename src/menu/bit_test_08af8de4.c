/* Whether bit `bit` of the second unlock mask is set. */
typedef struct BitTestState {
    char pad_0000[0x29F8];
    unsigned int field_29F8;
} BitTestState;

typedef struct BitTestOwner {
    char pad_000[0x140];
    BitTestState *field_140;
} BitTestOwner;

int func_08AF8DE4(BitTestOwner *self, short bit)
{
    int result = 0;
    if ((self->field_140->field_29F8 & (1 << bit)) != 0) {
        result = 1;
    }
    return result;
}
