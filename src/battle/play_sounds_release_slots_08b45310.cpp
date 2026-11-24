// Posts four sound/effect requests then releases the owner's six active slots.

struct Pool { char pad[0x48]; short ids[6]; };
extern void *D_089D4960;
extern "C" int func_089110B4(void);
extern "C" void func_0884417C(void *, int, int, int, int);
extern "C" struct Pool *func_089132A4(int);
extern "C" void func_089121F4(int);

struct Post {
    static void run(int id, int v) { func_0884417C(D_089D4960, id, v, 0x264000, 0); }
};

extern "C" void func_08B45310(int *self)
{
    int *keep[1];
    keep[0] = self;
    Post::run(0x54E, func_089110B4());
    Post::run(0x134, func_089110B4());
    Post::run(0x408, func_089110B4());
    Post::run(0x548, func_089110B4());
    self = keep[0];
    struct Pool *pool = func_089132A4(*self);
    int i = 0;
    short *p = pool->ids;
    do {
        short v = *p;
        if (v >= 0) {
            func_089121F4(v);
        }
        i++;
        p++;
    } while (i < 6);
}
