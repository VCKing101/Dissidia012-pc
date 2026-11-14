/* Runs the handler of the duel phase's current state, forwarding the time
 * step or the frame scale the handler needs. */
typedef struct DuelPhaseState {
    void *vtable;
    unsigned char pad_04[4];
    unsigned int state;
} DuelPhaseState;

extern int func_089F48C8(DuelPhaseState *phase, float step);
extern int func_089F4920(DuelPhaseState *phase, float scale);
extern int func_089F4968(DuelPhaseState *phase, float scale);
extern int func_089F49FC(DuelPhaseState *phase, float scale);
extern int func_089F4C54(DuelPhaseState *phase, float step);
extern int func_089F4D68(DuelPhaseState *phase, float scale);
extern int func_089F4E20(DuelPhaseState *phase, float scale);
extern int func_089F4E30(DuelPhaseState *phase, float step);

int func_089F4800(DuelPhaseState *phase, float step, float scale)
{
    switch (phase->state) {
    case 0:
        return func_089F48C8(phase, step);
    case 1:
        return func_089F4920(phase, scale);
    case 2:
        return func_089F4968(phase, scale);
    case 3:
        return func_089F49FC(phase, scale);
    case 4:
        return func_089F4C54(phase, step);
    case 5:
        return func_089F4D68(phase, scale);
    case 6:
        return func_089F4E20(phase, scale);
    case 7:
        return func_089F4E30(phase, step);
    }
    return 0;
}
