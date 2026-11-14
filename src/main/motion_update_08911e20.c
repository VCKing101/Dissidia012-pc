extern void func_08911A2C(void *motion);
extern void func_08911FB8(void *motion);

typedef struct MotionState {
    char pad_00[0xD];
    signed char kind;           /* 0x0D */
} MotionState;

void func_08911E20(MotionState *state)
{
    func_08911A2C(state);
    switch (state->kind) {
    case 0:
    case 3:
        break;
    case 1:
    case 2:
        func_08911FB8(state);
        break;
    }
}
