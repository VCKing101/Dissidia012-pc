/* Race phase, running state: shows the race progress and tag fade from the
 * race status, or closes the displays and moves to state 3 when the race is
 * over. */
typedef struct RaceStatus {
    unsigned char pad_00[8];
    float distance;
    unsigned char pad_0c[8];
    int lead;
    unsigned char pad_18[0xC];
    unsigned char leading;
    unsigned char pad_25[3];
    float gap;
} RaceStatus;

typedef struct RacePhaseData {
    unsigned char pad_00[8];
    int state;
    unsigned char pad_0c[8];
    unsigned char cue[0x28];
    unsigned char progress[0x30];
    unsigned char lead_meter[0x34];
    unsigned char tag[0x1C];
    int wins;
    int losses;
    void *rules;
} RacePhaseData;

extern int func_08A22E2C(RaceStatus *status, int wins, int losses, int kind);
extern void func_089FE1BC(void *meter, int lead);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *progress, float ratio);
extern int func_089FCA0C(void *tag, float target);
extern void func_08A01C74(void *cue, int kind);
extern void func_08A16C58(void *progress);
extern void func_089FC9A0(void *tag);

int func_089FD22C(RacePhaseData *phase)
{
    RaceStatus status;

    if (func_08A22E2C(&status, phase->wins, phase->losses, 0x1A) == 0) {
        float length;

        if (status.leading == 1) {
            func_089FE1BC(phase->lead_meter, -status.lead);
        }
        length = func_08891348(phase->rules);
        if (length != 0.0f) {
            func_08A16CC4(phase->progress, status.distance / length);
        }
        func_089FCA0C(phase->tag, 1.0f - status.gap);
    } else {
        func_08A01C74(phase->cue, 6);
        func_08A16C58(phase->progress);
        func_089FC9A0(phase->tag);
        phase->state = 3;
    }
    return 0;
}
