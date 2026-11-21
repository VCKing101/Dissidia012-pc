/* AI condition 3 (attack level 0-5) on the fighter alone: cues the guard
 * while held, the attack level when it can act, or the guard again while
 * thrown (-1 bad arguments, -2 other condition or no fighter). */
typedef struct HeldCond {
    unsigned char pad_00[8];
    int kind;
    unsigned int level;
} HeldCond;

typedef struct HeldInfo {
    unsigned char pad_00[4];
    void *fighter;
} HeldInfo;

typedef struct HeldAi {
    unsigned char pad_00[0x64];
    void *owner;
    void *cues;
} HeldAi;

extern HeldInfo *func_08AB3264(void *owner);
extern int func_088374B0(void *fighter);
extern void func_08A969BC(void *cues, int cue);
extern int func_08AB344C(void *fighter);
extern void func_08A96BAC(void *cues, unsigned int level);
extern int func_08B2F7E0(void *fighter);

int func_08A711D8(HeldAi *self, HeldCond *cond, void *key, void *flag)
{
    void *fighter;
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    if (cond->kind != 3 || cond->level >= 6) {
        return -2;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (func_088374B0(fighter) == 0) {
        func_08A969BC(self->cues, 0);
    } else {
        int able = func_08AB344C(fighter);
        int one = 1;
        if (able == one) {
            func_08A96BAC(self->cues, cond->level);
        } else if (func_08B2F7E0(fighter) == one) {
            func_08A969BC(self->cues, 0);
        }
    }
    return 0;
}
