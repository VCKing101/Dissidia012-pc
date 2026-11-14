typedef struct EffectInfo {
    short id;
    char pad2[0x8];
    short flags;
} EffectInfo;
typedef struct Effect {
    char pad0[0xC];
    unsigned char active;
    char padD[0x3];
    int state;
    void *owner;
    char pad18[0x30];
    EffectInfo *info;
} Effect;
extern int func_08899300(void);
extern void func_088983C8(int manager, int id, int mode);
extern void func_0881559C(void *owner);
int func_08818DB0(Effect *effect)
{
    if (effect->active == 0) {
        return -1;
    }
    if (effect->state != 4) {
        return 0;
    }
    if (effect->info->flags & 1) {
        func_088983C8(func_08899300(), effect->info->id, 1);
        if (effect->owner != 0) {
            func_0881559C(effect->owner);
        }
    }
    return 0;
}
