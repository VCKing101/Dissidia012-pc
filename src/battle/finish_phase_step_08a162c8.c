/* Runs the handler of the finish phase's current state. */
typedef struct FinishPhase {
    unsigned char pad_00[8];
    int state;
} FinishPhase;

extern int func_08A16378(FinishPhase *phase, float dt);
extern int func_08A163C8(FinishPhase *phase, float dt);
extern int func_08A1646C(FinishPhase *phase, float dt);
extern int func_08A164E0(FinishPhase *phase, float dt);
extern int func_08A16500(FinishPhase *phase, float dt);
extern int func_08A16550(FinishPhase *phase, float dt);
extern int func_08A165A8(FinishPhase *phase, float dt);

int func_08A162C8(FinishPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A16378(phase, x);
    case 1:
        return func_08A163C8(phase, x);
    case 2:
        return func_08A1646C(phase, x);
    case 3:
        return func_08A164E0(phase, x);
    case 4:
        return func_08A16500(phase, x);
    case 5:
        return func_08A16550(phase, x);
    case 6:
        return func_08A165A8(phase, x);
    }
    return 0;
}
