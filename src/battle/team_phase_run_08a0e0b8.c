/* Team phase, running states: show the team progress from the status and
 * advance the phase when the lead changes or the race ends. */
typedef struct TeamStatus {
    unsigned char pad_00[8];
    float distance;
    unsigned char pad_0c[0x18];
    unsigned char leading;
    unsigned char pad_25[7];
} TeamStatus;

typedef struct TeamPhaseRun {
    unsigned char pad_00[8];
    int state;
    int delay;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_12[6];
    unsigned char cue[0x28];
    unsigned char progress[0x30];
    unsigned char mark[0x24];
    unsigned char tag[0x14];
    int wins;
    int losses;
    void *rules;
} TeamPhaseRun;

extern int func_08A22E2C(TeamStatus *status, int wins, int losses, int kind);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *progress, float ratio);
extern void func_08A01C74(void *cue, int kind);
extern void func_08A16C58(void *progress);
extern int func_08A020C8(void *mark);
extern int func_08A0D8AC(void *tag);
extern void func_08889F28(int side, int pad);

int func_08A0E0B8(TeamPhaseRun *phase)
{
    TeamStatus status;

    if (func_08A22E2C(&status, phase->wins, phase->losses, 1) == 0) {
        float length;

        if (status.leading == 1) {
            phase->state = 4;
        }
        length = func_08891348(phase->rules);
        if (length != 0.0f) {
            func_08A16CC4(phase->progress, status.distance / length);
        }
    } else {
        func_08A01C74(phase->cue, 5);
        func_08A16C58(phase->progress);
        phase->state = 3;
    }
    return 0;
}
