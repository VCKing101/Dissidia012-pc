typedef struct ShortTable {
    short field_00;
    unsigned short field_02[5];
} ShortTable;

extern ShortTable *D_08A29BF0;

unsigned short func_08A2969C(int index)
{
    if (D_08A29BF0 != 0) {
        if (index < 0 || index >= 5) {
            index = 0;
        }
        return D_08A29BF0->field_02[index];
    }
    return 0;
}
