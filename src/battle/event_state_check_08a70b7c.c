/* Marks the event when the owner is in none of states 5 and 7, and not in
 * state 6 unless also in state 8 (-1 bad arguments). */
typedef struct StateEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char pad_02[0x62];
    void *owner;
} StateEvent;

extern void *func_08AB3274(void *owner);
extern int func_08A82638(void *state, int which);

int func_08A70B7C(StateEvent *self, void *key, void *context, void *flag)
{
    void *state;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    state = func_08AB3274(self->owner);
    if (func_08A82638(state, 5) == 0 && func_08A82638(state, 7) == 0 &&
        (func_08A82638(state, 6) == 0 || func_08A82638(state, 8) == 1)) {
        self->marked = 1;
    }
    return 0;
}
