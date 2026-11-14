typedef struct MatchKey {
    char pad_00[8];
    unsigned short field_08;
} MatchKey;

typedef struct MatchCell {
    char pad_00[0xA];
    short field_0A;
    char field_0B_pad;
} MatchCell;

typedef struct MatchCellB {
    char pad_00[0xB];
    char field_0B;
} MatchCellB;

typedef struct MatchCtx {
    char pad_00[0x68];
    MatchCell *field_68;
    MatchCellB *field_6C;
} MatchCtx;

typedef struct MatchOwner {
    char pad_00[0x14C];
    MatchCtx *field_14C;
} MatchOwner;

int func_089F0B54(MatchOwner *self, MatchKey *key)
{
    int result;
    if (self->field_14C->field_6C->field_0B == 0) {
        return 0;
    }
    result = 0;
    if ((unsigned int)key->field_08 == self->field_14C->field_68->field_0A) {
        result = 1;
    }
    return result;
}
