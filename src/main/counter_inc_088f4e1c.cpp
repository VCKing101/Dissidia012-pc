/* Increments the counter at 0x28, capped at 9999. */
struct Counter {
    char pad_00[0x28];
    short count;

    static int clamp(int value, int max)
    {
        return value > max ? max : value;
    }
};

extern "C" {
void func_088F4E1C(Counter *c)
{
    c->count = Counter::clamp(c->count + 1, 9999);
}
}
