/* Returns the first or second value of pair `index` of D_08B281D8. */
struct Pair08B27FB4 {
    short field_0;
    short field_2;
};

extern struct Pair08B27FB4 D_08B281D8[];

int func_08B27FB4(int index, int second)
{
    if (second == 0) {
        return D_08B281D8[index].field_0;
    } else {
        return D_08B281D8[index].field_2;
    }
}
