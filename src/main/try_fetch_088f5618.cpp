// Fetch four values for a row; copy them out and report success when the fetch returned 1.
struct Vals { int a, b, c, d; };
extern "C" int func_088F6554(void *a, Vals *vals, int row);
extern "C" bool func_088F5618(int self, int *o1, int *o2, int *o3, int *o4, int index)
{
    int buf[1];
    Vals v;
    int r = func_088F6554(buf, &v, self + index * 0x10A8);
    if (r == 1) {
        *o1 = v.a;
        *o2 = v.b;
        *o3 = v.c;
        *o4 = v.d;
        return true;
    }
    return false;
}
