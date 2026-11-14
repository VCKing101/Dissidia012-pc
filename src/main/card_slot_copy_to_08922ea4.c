typedef struct CardSlot { short state; short count; short level; short id; int values[5]; int field_1C; } CardSlot;
void func_08922EA4(CardSlot *src, CardSlot *dst)
{
    int i;
    dst->state = src->state;
    dst->count = src->count;
    dst->level = src->level;
    dst->id = src->id;
    for (i = 0; i < 5; i++) {
        dst->values[i] = src->values[i];
    }
    dst->field_1C = src->field_1C;
}
