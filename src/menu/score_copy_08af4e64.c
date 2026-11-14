/* Copies both thirteen-entry score rows. */
typedef struct CopyScores {
    int field_00[13];
    int field_34[13];
} CopyScores;

void func_08AF4E64(void *self, CopyScores *src, CopyScores *dst)
{
    int i;
    for (i = 0; i < 13; i++) {
        dst->field_00[i] = src->field_00[i];
        dst->field_34[i] = src->field_34[i];
    }
}
