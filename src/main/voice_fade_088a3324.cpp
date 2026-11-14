/* Updates the voice's channel and starts its fade once. */
struct Channel088A3324 {
    int field_00;
    char inner[4];
};

struct Voice088A3324 {
    int field_00;
    int field_04;
    Channel088A3324 *channel;
    unsigned int flags;

    static bool has(Voice088A3324 *v, unsigned int mask) { return (v->flags & mask) != 0; }
};

extern "C" {

void func_0889F948(void *inner, float a);
void func_0889F8D4(void *inner, float time, int mode);

void func_088A3324(Voice088A3324 *v, float a, float time)
{
    func_0889F948(v->channel->inner, a);
    if (!Voice088A3324::has(v, 2)) {
        func_0889F8D4(v->channel->inner, time, 0);
        v->flags |= 2;
    }
}

}
