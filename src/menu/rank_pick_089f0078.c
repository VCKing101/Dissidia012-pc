typedef struct RankCtx {
    char pad_00[0x4C];
    short *field_4C;
} RankCtx;

typedef struct RankOwner {
    char pad_00[0x14C];
    RankCtx *field_14C;
} RankOwner;

unsigned char func_089F0078(RankOwner *self, unsigned short score)
{
    unsigned char rank = 0;
    int i;
    short *limits = self->field_14C->field_4C;
    for (i = 1; i < 6; i++) {
        if (score < limits[i]) {
            break;
        }
        rank++;
    }
    return rank;
}
