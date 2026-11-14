/* Seeds a random value with 1, then draws the next 15-bit value from the global generator. */
extern "C" int D_09AF84D0;
struct Rand088C1C0C {
    unsigned int value;

    static int next()
    {
        D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3;
        return (D_09AF84D0 >> 16) & 0x7FFF;
    }
};
extern "C" {
Rand088C1C0C *func_088C1C0C(Rand088C1C0C *r)
{
    r->value = 1;
    r->value = Rand088C1C0C::next();
    return r;
}
}
