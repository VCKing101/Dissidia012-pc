/* Ten minus the value's tens, clamped to 0..10. */
struct Clamp088C0640 {
    static int range(int v, int lo, int hi)
    {
        if (v < lo) {
            return lo;
        }
        if (v > hi) {
            return hi;
        }
        return v;
    }
};
extern "C" {
int func_088C0640(int value)
{
    return 10 - Clamp088C0640::range(value / 10, 0, 10);
}
}
