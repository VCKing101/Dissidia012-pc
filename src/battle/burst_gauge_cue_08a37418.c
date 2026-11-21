/* Burst ready cue: shows the burst gauge (and sets the fighter's burst
 * flags) while the first stage runs or the second is under 58 frames;
 * hides it and clears the flags otherwise. */
typedef struct CueFighter {
    char pad_000[0x388];
    unsigned long long flags;
    char pad_390[0x3C];
    float field_3CC;
} CueFighter;

typedef struct CueStage {
    char pad_00[0x8];
    int stage;
    float time;
} CueStage;

typedef struct CueReady {
    CueFighter *fighter;
    char pad_04[0x10];
    CueStage *stage;
} CueReady;

extern int D_08A62280;
extern unsigned long long D_08A38488;
extern void func_08A62020(int id, int value, float time);
extern void func_08A620B0(int id, int value);

void func_08A37418(CueReady *self)
{
    int show = 0;
    switch (self->stage->stage) {
    case 0:
        show = 1;
        break;
    case 1:
        if (self->stage->time < 58.0f) {
            show = 1;
        }
        break;
    }
    if (show) {
        CueFighter *fighter;
        func_08A62020(D_08A62280, 0, 0.0f);
        fighter = self->fighter;
        fighter->field_3CC = 2.0f;
        fighter->flags |= 0x80000000ULL;
    } else {
        func_08A620B0(D_08A62280, 0);
        self->fighter->flags &= D_08A38488;
    }
}
