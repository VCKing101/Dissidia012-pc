/* Returns field 4 of entry `index` of D_08B28010. */
struct Entry08B27FDC {
    short field_0;
    short field_2;
    short field_4;
    short field_6;
};

extern struct Entry08B27FDC D_08B28010[];

int func_08B27FDC(int index)
{
    return D_08B28010[index].field_4;
}
