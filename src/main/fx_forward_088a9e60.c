/* Forwards its arguments with the pair passed by address. */
typedef struct FxPair088A9E60 {
    int first;
    int second;
} FxPair088A9E60;

void func_088AA6F8(int a, int b, int c, FxPair088A9E60 *pair, int d, int e, int f);

void func_088A9E60(int a, int b, int c, int first, int second, int d, int e, int f)
{
    FxPair088A9E60 pair;
    pair.first = first;
    pair.second = second;
    func_088AA6F8(a, b, c, &pair, d, e, f);
}
