/* Stage phase, running state: shows the lead and progress from the stage
 * status, or closes the stage displays and moves to state 5 once the stage
 * is over. */
typedef struct StageStatus {
    unsigned char pad_00[8];
    float distance;
    unsigned char pad_0c[0x18];
    unsigned char leading;
    unsigned char pad_25[7];
} StageStatus;

typedef struct StageRecord {
    unsigned char pad_00[0x30];
    short lead;
    unsigned char pad_32[0xE];
} StageRecord;

typedef struct StagePhaseRun {
    unsigned char pad_00[8];
    int state;
    unsigned char pad_0c[0xC0];
    unsigned char cue[0x28];
    unsigned char counter[0x14];
    unsigned char progress[0x30];
    unsigned char meter[0x34];
    unsigned char score[0x10C];
    int wins;
    int losses;
    void *rules;
} StagePhaseRun;

extern int func_08A22E2C(StageStatus *status, int wins, int losses, int kind);
extern int func_08891710(void *rules, StageRecord *record, int kind);
extern void func_089FE1BC(void *meter, int lead);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *progress, float ratio);
extern unsigned int func_08A22CA8(int turn);
extern void func_08A01C74(void *cue, int kind);
extern int func_08891684(void *rules);
extern void func_08A11200(void *counter, int value);
extern void func_08A16C58(void *progress);
extern int func_089FC308(void *score);
extern int D_089FFC7C[2];

int func_089FF930(StagePhaseRun *phase)
{
    StageStatus status;

    if (func_08A22E2C(&status, phase->wins, phase->losses, 3) == 0) {
        float length;

        if (status.leading == 1) {
            StageRecord record;
            if (func_08891710(phase->rules, &record, 3) != 0) {
                return -1;
            }
            func_089FE1BC(phase->meter, record.lead);
        }
        length = func_08891348(phase->rules);
        if (length != 0.0f) {
            func_08A16CC4(phase->progress, status.distance / length);
        }
    } else {
        unsigned int side = func_08A22CA8((phase->wins + phase->losses) % 2);
        if (side >= 2) {
            return -2;
        }
        func_08A01C74(phase->cue, 11);
        if (func_08891684(phase->rules) == 0x27) {
            func_08A11200(phase->counter, D_089FFC7C[side]);
        }
        func_08A16C58(phase->progress);
        func_089FC308(phase->score);
        phase->state = 5;
    }
    return 0;
}
