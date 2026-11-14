typedef struct EffectInfo {
    short id;
} EffectInfo;
typedef struct Effect {
    char pad0[0xC];
    unsigned char active;
    char padD[0x3];
    int state;
    void *owner;
    char pad18[0xC];
    unsigned char started;
    char pad25[0x3];
    float time;
    int frame;
    unsigned char looped;
    char pad31[0x3];
    float rate;
    int count;
    float delay;
    int target;
    unsigned char paused;
    unsigned char done;
    char pad46[0x2];
    EffectInfo *info;
} Effect;
extern int func_08899300(void);
extern void func_088981B4(int manager, int id, int a, int b);
extern void func_0881559C(void *owner);
extern int func_0881BAA8(void);
extern void func_0881CB3C(int manager, int id, int a);
int func_08819080(Effect *effect)
{
    int result = -1;
    if (effect->active == 0) {
        return result;
    }
    {
        effect->state = 3;
        effect->started = 0;
        effect->time = 0.0f;
        effect->frame = 0;
        effect->looped = 0;
        effect->rate = 0.0f;
        effect->count = 0;
        effect->delay = 0.0f;
        effect->target = result;
        effect->paused = 0;
        effect->done = 0;
        func_088981B4(func_08899300(), effect->info->id, 0, 1);
        if (effect->owner != 0) {
            func_0881559C(effect->owner);
        }
        func_0881CB3C(func_0881BAA8(), effect->info->id, 0);
    }
    return 0;
}
