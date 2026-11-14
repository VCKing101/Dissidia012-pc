typedef struct UnitInfo {
    char pad_00[0x4];
    void *body;
} UnitInfo;

typedef struct TargetOwner {
    unsigned char active;
    unsigned char missing;
    char pad_02[0x62];
    int unit;
} TargetOwner;

extern UnitInfo *func_08AB3264(int unit);
extern void *func_08B30898(void *body);
extern int func_08A6B4E4(TargetOwner *self, void *target, int mode);

int func_08A6D664(TargetOwner *self)
{
    UnitInfo *info = func_08AB3264(self->unit);
    void *target;
    if (info->body == 0) {
        return -1;
    }
    target = func_08B30898(info->body);
    if (target == 0) {
        self->missing = 1;
        return 0;
    }
    if (func_08A6B4E4(self, target, 0) != 0) {
        return -2;
    }
    return 0;
}
