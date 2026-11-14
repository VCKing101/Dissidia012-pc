typedef struct Model { int field_00; int field_04; void *skeleton; } Model;
typedef struct Actor { char data_00[0x84]; Model *model; } Actor;

extern int func_089B0D2C(void *skeleton);
extern void *func_089B0F44(void *skeleton, int index);
extern int func_089B185C(void *bone);

int func_0886B464(Actor *actor, int index)
{
    if (index < 0) {
        index = func_089B0D2C(actor->model->skeleton);
    }
    return func_089B185C(func_089B0F44(actor->model->skeleton, index)) == 1;
}
