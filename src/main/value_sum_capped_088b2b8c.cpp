/* Sum of a value pair, capped at 999999999. */
struct Pair088B2B8C {
    int a;
    int b;

    static const int &min(const int &x, const int &y)
    {
        return y < x ? y : x;
    }
};

extern "C" {

int func_088B2B8C(Pair088B2B8C *pair)
{
    int sum = pair->a + pair->b;
    int cap = 999999999;
    return Pair088B2B8C::min(sum, cap);
}

}
