/* Returns the first or second value of pair `index` of D_08B04908. */
struct Pair08B045D0 {
    short field_0;
    short field_2;
};

extern struct Pair08B045D0 D_08B04908[];

int func_08B045D0(int index, int second)
{
    if (second == 0) {
        return D_08B04908[index].field_0;
    } else {
        return D_08B04908[index].field_2;
    }
}
