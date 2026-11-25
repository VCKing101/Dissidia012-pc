/* Whether the fighter can use its burst follow-up (skill 0x11D): it needs
 * the skill, a ready check, a state other than 0x13 and 0x19, enough
 * points, and no cost or a cost within its limit. */
typedef struct FollowCost {
    short value;
} FollowCost;

typedef struct FollowSkill {
    unsigned char pad_00[4];
    FollowCost cost;
} FollowSkill;

typedef struct FollowState {
    volatile int kind;
} FollowState;

typedef struct FollowFighter {
    unsigned char pad_0000[0x3A4];
    FollowState *state;
    unsigned char pad_03a8[0xCCC];
    unsigned char points;
    unsigned char pad_1075;
    unsigned char limit;
} FollowFighter;

extern void *func_08835778(FollowFighter *fighter);
extern FollowSkill *func_088BF4CC(void *skills, int id);
extern int func_08B305E8(FollowFighter *fighter);
extern int func_08B2F794(FollowFighter *fighter);

int func_08A50AF4(FollowFighter *self)
{
    FollowFighter *fighter = self;
    FollowSkill *skill;
    FollowState *state;
    short cost;
    int c13;
    if ((unsigned char)(func_088BF4CC(func_08835778(self), 0x11D) != 0) == 0) {
        return 0;
    }
    if (func_08B305E8(self) == 0) {
        return 0;
    }
    state = self->state;
    c13 = 0x13;
    if (state->kind == c13) {
        return 0;
    }
    if (state->kind == 0x19) {
        return 0;
    }
    if (fighter->points < func_08B2F794(fighter)) {
        return 0;
    }
    skill = func_088BF4CC(func_08835778(self), 0x11D);
    cost = 0;
    if (skill != 0) {
        FollowCost *entry = &skill->cost;
        cost = entry->value;
    }
    if (cost != 0 && fighter->limit >= cost) {
        return 0;
    }
    return 1;
}
