/* The current rank mask when bit `bit` of the unlock mask is set, else 0. */
typedef struct BitRankState {
    char pad_0000[0x29F4];
    unsigned int field_29F4;
} BitRankState;

typedef struct BitRankOwner {
    char pad_000[0x140];
    BitRankState *field_140;
} BitRankOwner;

extern int func_08AF3B64(BitRankOwner *self);

int func_08AF8DA4(BitRankOwner *self, short bit)
{
    int result = 0;
    if ((self->field_140->field_29F4 & (1 << bit)) != 0) {
        result = func_08AF3B64(self);
    }
    return result;
}
