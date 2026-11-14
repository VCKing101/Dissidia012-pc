typedef struct MotionFlags {
    unsigned int playing : 1;
    unsigned int looping : 1;
    unsigned int paused : 1;
    unsigned int reverse : 1;
    unsigned int blend : 1;
    unsigned int layer : 8;
    unsigned int rest : 19;
} MotionFlags;
typedef struct Motion {
    char a[0x10];
    char b[0x10];
    MotionFlags flags;
    int f24;
    int f28;
    int f2C;
    unsigned char b30;
    unsigned char b31;
    unsigned char b32;
    unsigned char b33;
} Motion;
extern void func_089A3890(void *p);
void func_08822324(Motion *m)
{
    m->f24 = 0;
    m->flags.playing = 0;
    m->flags.looping = 0;
    m->flags.paused = 0;
    m->flags.reverse = 0;
    m->flags.blend = 0;
    m->flags.layer = 0;
    m->f28 = 0;
    m->b30 = 0;
    m->f2C = 0;
    func_089A3890(m->a);
    func_089A3890(m->b);
    m->b31 = 0;
    m->b32 = 0;
    m->b33 = 0;
}
