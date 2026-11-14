/* Starts the follow-up (mode 1, step 2) when the owner is in state 1, the
 * fighter's motion has no frames left to play and the player is in mode 3
 * (-1 bad arguments, -2 no fighter). */
typedef struct ThrowFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[4];
} ThrowFighter;

typedef struct ThrowInfo {
    unsigned char pad_00[4];
    ThrowFighter *fighter;
} ThrowInfo;

typedef struct ThrowEvent {
    unsigned char pad_00[0x64];
    void *owner;
} ThrowEvent;

extern ThrowInfo *func_08AB3264(void *owner);
extern void *func_08AB327C(void *owner);
extern void *func_08AB3274(void *owner);
extern int func_08A82684(void *state, int which);
extern int func_0882FE7C(float x, float y, void *motion, int a, int b);
extern int func_08AB1B50(void *player, int mode);
extern void func_08A97910(ThrowInfo *info, int mode, int step);

int func_08A72754(ThrowEvent *self, void *key, void *context, void *flag)
{
    ThrowFighter *fighter;
    void *player;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    player = func_08AB327C(self->owner);
    if (func_08A82684(func_08AB3274(self->owner), 1) == 1 && func_0882FE7C(0.0f, 0.0f, fighter->motion, 0, 0) == 0 &&
        func_08AB1B50(player, 3) == 1) {
        func_08A97910(func_08AB3264(self->owner), 1, 2);
    }
    return 0;
}
