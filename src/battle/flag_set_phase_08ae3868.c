typedef struct SceneState {
    char pad_00[0xC];
    int phase;
} SceneState;

typedef struct FlagSet3 {
    char pad_00[0x24];
    int a;
    int b;
    int c;
} FlagSet3;

extern SceneState *D_08AE7CA0;

int func_08AE3868(FlagSet3 *self)
{
    int result = 0;
    if ((self->a | self->b | self->c) == 0) {
        result = D_08AE7CA0->phase;
    }
    return result;
}
