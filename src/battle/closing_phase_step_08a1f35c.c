/* Runs the handler of the closing phase's current state. */
typedef struct ClosingPhase {
    unsigned char pad_00[8];
    int state;
} ClosingPhase;

extern int func_08A1F40C(ClosingPhase *phase, float dt);
extern int func_08A1F45C(ClosingPhase *phase, float dt);
extern int func_08A1F4AC(ClosingPhase *phase, float dt);
extern int func_08A1F51C(ClosingPhase *phase, float dt);
extern int func_08A1F55C(ClosingPhase *phase, float dt);
extern int func_08A1F5AC(ClosingPhase *phase, float dt);
extern int func_08A1F604(ClosingPhase *phase, float dt);

int func_08A1F35C(ClosingPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A1F40C(phase, x);
    case 1:
        return func_08A1F45C(phase, x);
    case 2:
        return func_08A1F4AC(phase, x);
    case 3:
        return func_08A1F51C(phase, x);
    case 4:
        return func_08A1F55C(phase, x);
    case 5:
        return func_08A1F5AC(phase, x);
    case 6:
        return func_08A1F604(phase, x);
    }
    return 0;
}
