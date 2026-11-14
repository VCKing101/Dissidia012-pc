/* Bit mask of the current rank (rank 1 is bit 0). */
typedef struct RankBitState {
    int field_00;
} RankBitState;

typedef struct RankBitOwner {
    char pad_000[0x140];
    RankBitState *field_140;
} RankBitOwner;

int func_08AF3B64(RankBitOwner *self)
{
    return 1 << (self->field_140->field_00 - 1);
}
