/* Copies the vector at 0x09AF84C0 and passes it to func_08B5DE48 with two zero floats. */
struct V3 {
    float x, y, z;
    static V3 Copy(const V3 *p) { return *p; }
};
extern "C" {
extern V3 D_09AF84C0;
extern void func_08B5DE48(int, V3 *, int, float, float);
void func_08B5D99C(int arg)
{
    const V3 *p = &D_09AF84C0;
    V3 b = V3::Copy(p);
    func_08B5DE48(arg, &b, 0, 0.0f, 0.0f);
}
}
