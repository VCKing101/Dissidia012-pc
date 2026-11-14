/* Rank of the character entry: 1 when unknown, 0 for an unranked placed entry, else the table value. */
typedef struct RankInfo {
    char pad_00[0xA];
    short field_0A;
    char pad_0C[0x14 - 0xC];
    signed char field_14;
} RankInfo;

extern int D_08B01630[];
extern RankInfo *func_088B29D8(int id);

int func_08AF3D68(void *self, short id)
{
    int result = 1;
    RankInfo *info = func_088B29D8(id);
    if (info != 0) {
        if (info->field_14 == 0 && info->field_0A != -1) {
            result = 0;
        } else {
            result = D_08B01630[info->field_14];
        }
    }
    return result;
}
