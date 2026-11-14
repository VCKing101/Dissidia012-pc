typedef struct Table {
    short field_00;
    unsigned short shortSizes[3];
    union { unsigned int wordSizes[1]; unsigned short halfSizes[2]; } u;
} Table;
extern int func_0887A8F0(Table *t);
extern int func_0887A918(Table *t);

int func_0887A800(Table *t, int index, int count)
{
    int size = 0;
    if (index >= 0 && index < count) {
        if (func_0887A8F0(t) != 0) {
            size = *(unsigned int *)((char *)t->u.wordSizes + index * 4) << 2;
        } else if (func_0887A918(t) != 0) {
            size = *(unsigned short *)((char *)t->u.halfSizes + index * 2) * 2;
        } else {
            size = *(unsigned short *)((char *)t->shortSizes + index * 2) * 2;
        }
    }
    return size;
}
