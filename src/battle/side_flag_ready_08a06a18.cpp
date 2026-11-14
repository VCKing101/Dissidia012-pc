/* True when the side flag may be raised: always while the battle root or
 * the owner is missing, otherwise when the rules allow it or the flag is
 * forced. */
typedef struct RootHolder {
    unsigned char pad_000[0x7A4];
    void *root;
} RootHolder;

typedef struct FlagRules {
    unsigned char open;
    unsigned char pad_01[3];
    int target;
    unsigned char pad_08[8];
    int forced;
    unsigned char pad_14[8];
    int owner;
    unsigned char pad_20[8];
    void *rules;
} FlagRules;

extern "C" RootHolder *D_089D6A10;

struct FlagCheck {
    static int allowed(FlagRules *flag, int known, int ready, int mode, int busy)
    {
        int result = 0;
        if ((((mode == 1 && busy == 0) || flag->target != -1) && known == 1 && ready == 1) || flag->forced == 1) {
            result = 1;
        }
        return result;
    }
};
extern "C" int func_08A22C78(int owner);
extern "C" int func_08891684(void *rules);
extern "C" int func_089DDB70(void *root);
extern "C" int func_0889120C(void *rules);
extern "C" int func_08891944(void *rules);

extern "C" unsigned char func_08A06A18(FlagRules *flag)
{
    void *root = D_089D6A10->root;
    int known;
    int ready;
    int mode;
    int busy;
    int result;

    if (root == 0) {
        return 1;
    }
    if (func_08A22C78(flag->owner) == 0) {
        return 1;
    }
    known = func_08891684(flag->rules) >= 0;
    ready = func_089DDB70(root);
    mode = func_0889120C(flag->rules);
    busy = func_08891944(flag->rules);
    result = FlagCheck::allowed(flag, known, ready, mode, busy);
    return result;
}
