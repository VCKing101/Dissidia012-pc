typedef struct IdPair {
    short id;
    short value;
} IdPair;

typedef struct IdPairs {
    IdPair pairs[10];
    short count;                /* 0x28 */
    short field_2A;
} IdPairs;

void func_0891A254(IdPairs *list)
{
    int i;
    for (i = 0; i < 10; i++) {
        list->pairs[i].id = -1;
        list->pairs[i].value = 0;
    }
    list->count = 0;
    list->field_2A = 0;
}
