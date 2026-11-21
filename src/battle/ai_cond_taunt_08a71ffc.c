/* AI taunt condition (timer 13): picks one of five taunt cues when the
 * timer starts; met once the fighter has been in state 0x13; otherwise
 * shows the picked taunt cue (0x18, 0x17, 0x1A, 0x19 or none) and cue 0x15
 * (-1 bad arguments, -2 no fighter). */
typedef struct TauntState {
    int kind;
} TauntState;

typedef struct TauntFighter {
    unsigned char pad_000[0x3A4];
    TauntState *state;
} TauntFighter;

typedef struct TauntInfo {
    unsigned char pad_00[4];
    TauntFighter *fighter;
} TauntInfo;

typedef struct TauntAi {
    unsigned char pad_000;
    unsigned char met;
    unsigned char pad_002[0x12];
    unsigned char timers[0x50];
    void *owner;
    void *cues;
    unsigned char pad_06c[0x16C];
    int taunt;
    unsigned char seen;
} TauntAi;

extern void *D_08AA5828;

extern TauntInfo *func_08AB3264(void *owner);
extern int func_08A7E904(void *timers, int id);
extern void func_08A7E858(void *timers, int id);
extern int func_08AA5748(void *random);
extern void func_08A97060(void *cues, int cue);

int func_08A71FFC(TauntAi *self, void *cond, void *key, void *flag)
{
    TauntFighter *fighter;
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    {
        int id = 0xD;
        void *timers = self->timers;
        if (func_08A7E904(timers, id) == 0) {
            self->taunt = func_08AA5748(D_08AA5828) % 5;
            self->seen = 0;
        }
        func_08A7E858(timers, id);
        if (fighter->state->kind != 0x13) {
            if (self->seen == 1) {
                self->met = 1;
                return 0;
            }
        } else {
            self->seen = 1;
        }
        switch (self->taunt) {
        case 0:
            func_08A97060(self->cues, 0x18);
            break;
        case 1:
            func_08A97060(self->cues, 0x17);
            break;
        case 2:
            func_08A97060(self->cues, 0x1A);
            break;
        case 3:
            func_08A97060(self->cues, 0x19);
            break;
        }
        func_08A97060(self->cues, 0x15);
    }
    return 0;
}
