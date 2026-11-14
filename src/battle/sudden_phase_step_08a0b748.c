/* Runs the handler of the sudden-death phase's current state, forwarding the time
 * step or the frame scale the handler needs. */
typedef struct DuelPhaseState {
    void *vtable;
    unsigned char pad_04[4];
    unsigned int state;
} DuelPhaseState;

extern int func_08A0B810(DuelPhaseState *phase, float step);
extern int func_08A0B868(DuelPhaseState *phase, float step);
extern int func_08A0B8D8(DuelPhaseState *phase, float scale);
extern int func_08A0B930(DuelPhaseState *phase, float scale);
extern int func_08A0B9D0(DuelPhaseState *phase, float scale);
extern int func_08A0BA10(DuelPhaseState *phase, float step);
extern int func_08A0BA68(DuelPhaseState *phase, float step);
extern int func_08A0BAD0(DuelPhaseState *phase, float scale);

int func_08A0B748(DuelPhaseState *phase, float step, float scale)
{
    switch (phase->state) {
    case 0:
        return func_08A0B810(phase, step);
    case 1:
        return func_08A0B868(phase, step);
    case 2:
        return func_08A0B8D8(phase, scale);
    case 3:
        return func_08A0B930(phase, scale);
    case 4:
        return func_08A0B9D0(phase, scale);
    case 5:
        return func_08A0BA10(phase, step);
    case 6:
        return func_08A0BA68(phase, step);
    case 7:
        return func_08A0BAD0(phase, scale);
    }
    return 0;
}
