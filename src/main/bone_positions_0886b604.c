typedef struct Model { int field_00; int field_04; void *skeleton; } Model;
typedef struct Actor { char data_00[0x84]; Model *model; char data_88[0xA4 - 0x88]; int boneA; int boneB; } Actor;

extern void *func_089B0E78(void *skeleton, int index);
extern char *func_089B1224(void *bone);
extern void func_089A3868(void *out, void *in);

int func_0886B604(Actor *actor, void *outA, void *outB)
{
    if (actor->boneA >= 0 && actor->boneB >= 0) {
        void *a = func_089B0E78(actor->model->skeleton, actor->boneA);
        void *b = func_089B0E78(actor->model->skeleton, actor->boneB);
        char *ma = func_089B1224(a);
        char *mb = func_089B1224(b);
        func_089A3868(outA, ma + 0x30);
        func_089A3868(outB, mb + 0x30);
        return 1;
    }
    return 0;
}
