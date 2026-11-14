/* Returns field 4 of entry `index` of D_08B04670. */
struct Entry08B045F8 {
    short field_0;
    short field_2;
    short field_4;
    short field_6;
};

extern struct Entry08B045F8 D_08B04670[];

int func_08B045F8(int index)
{
    return D_08B04670[index].field_4;
}
