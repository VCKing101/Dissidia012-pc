typedef struct UnitInfo {
    char pad_00[0x4];
    char *body;
    char pad_08[0x28];
    int mode;
} UnitInfo;

typedef struct IdleOwner {
    char pad_000[0x1C];
    float speed;
    char pad_020[0x140];
    int unit;
} IdleOwner;

extern UnitInfo *func_08AB3264(int unit);
extern int func_08ACAA74(void *motion);

int func_08A9CC98(IdleOwner *self)
{
    char *body = func_08AB3264(self->unit)->body;
    if (body == 0) {
        return 0;
    }
    if (func_08ACAA74(body + 0x784) == 0) {
        return 0;
    }
    if (func_08AB3264(self->unit)->mode == 2) {
        return 0;
    }
    if (!(self->speed <= 0.0f)) {
        return 0;
    }
    return 1;
}
