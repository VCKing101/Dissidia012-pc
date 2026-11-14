/* Applies an alpha and tint to a model, or clears both. */
typedef struct BattleRgba {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} BattleRgba;

extern "C" {
extern float D_089E68B4;
extern void func_088D9AEC(void *, float);
extern void func_088D9AF4(void *, void *);
}

struct BattleMath {
    static float clampf(float value, float low, float high)
    {
        if (value < low) return low;
        if (high < value) return high;
        return value;
    }
};

extern "C" int func_089E67D0(void *self, void *model, unsigned char enable, float alpha)
{
    if (!model) return -1;
    if (!enable) {
        BattleRgba off;
        off.r = 0;
        off.g = 0;
        off.b = 0;
        off.a = 0;
        func_088D9AEC(model, 0.0f);
        func_088D9AF4(model, &off);
    } else {
        BattleRgba on;
        on.r = 0x80;
        on.g = 0x80;
        on.b = 0x80;
        on.a = 0xFF;
        func_088D9AEC(model, BattleMath::clampf(alpha, 0.0f, D_089E68B4));
        func_088D9AF4(model, &on);
    }
    return 0;
}
