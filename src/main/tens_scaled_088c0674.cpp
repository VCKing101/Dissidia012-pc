/* Ten times the distance below 10, clamped to 0..150. */
struct Clamp088C0674 {
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
int func_088C0674(int value)
{
    return Clamp088C0674::range((10 - value) * 10, 0, 150);
}
}
