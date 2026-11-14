typedef struct UnitInfo {
    char pad_00[0x4];
    void *body;
} UnitInfo;

typedef struct BodyOwner {
    unsigned char active;
    unsigned char ready;
    char pad_02[0x62];
    int unit;
} BodyOwner;

extern UnitInfo *func_08AB3264(int unit);
extern int func_08837458(void *body);

int func_08A706D0(BodyOwner *self, void *a, void *b, void *c)
{
    UnitInfo *info;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    info = func_08AB3264(self->unit);
    if (info->body == 0) {
        return -2;
    }
    if (func_08837458(info->body) == 1) {
        self->ready = 1;
    }
    return 0;
}
