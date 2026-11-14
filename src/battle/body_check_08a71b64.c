typedef struct UnitInfo {
    char pad_00[0x4];
    void *body;
} UnitInfo;

typedef struct CheckOwner {
    unsigned char active;
    unsigned char hit;
    unsigned char miss;
    char pad_03[0x61];
    int unit;
    int field_68;
} CheckOwner;

typedef struct BodyAction {
    int kind;
} BodyAction;

typedef struct ActionBody {
    char pad_000[0x3A4];
    BodyAction *action;
} ActionBody;

typedef struct RangeInfo {
    char pad_00[0x30];
    float range;
} RangeInfo;

extern int D_08A80218;
extern UnitInfo *func_08AB3264(int unit);
extern int func_08A80200(int id);
extern RangeInfo *func_08AACB10(int id);
extern float func_08A66820(CheckOwner *self);
extern void func_08A97060(int target, int value);

int func_08A71B64(CheckOwner *self, void *a, void *b, void *c)
{
    ActionBody *body;
    RangeInfo *info;
    float value;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    body = (ActionBody *)func_08AB3264(self->unit)->body;
    if (body == 0) {
        return -2;
    }
    info = func_08AACB10(func_08A80200(D_08A80218));
    if (info == 0) {
        return -3;
    }
    value = func_08A66820(self);
    if (body->action->kind != 0x13 || value <= info->range) {
        self->hit = 1;
    } else {
        func_08A97060(self->field_68, 0x15);
    }
    return 0;
}
