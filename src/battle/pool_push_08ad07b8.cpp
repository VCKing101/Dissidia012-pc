/* Pushes the address onto the stack kept at D_089D4918 when it lies inside that pool's first 0x2600 bytes. */
extern "C" {
extern unsigned D_089D4918;
struct PS { char pad[0x2600]; unsigned items[32]; int count; };

struct PoolPush {
    static unsigned char In(unsigned p, unsigned base)
    {
        int r = 0;
        if (p >= base && p < base + 0x2600) {
            r = 1;
        }
        return r;
    }
};

void func_08AD07B8(unsigned p)
{
    unsigned base = D_089D4918;
    if (PoolPush::In(p, base) == 1) {
        int *c = &((PS *)D_089D4918)->count;
        int n = *c;
        unsigned off = n << 2;
        n++;
        *c = n;
        *(unsigned *)((char *)base + sizeof(((PS *)0)->pad) + off) = p;
    }
}
}
