typedef struct UnitInfo {
    char pad_00[0x60];
    char data[0x10];
} UnitInfo;

extern UnitInfo *func_08B30898(void *unit);
extern int func_08A82B8C(void *data, int size, int count);

int func_08A829D8(void *unit)
{
    UnitInfo *info;

    if (unit == 0) {
        return 0;
    }
    info = func_08B30898(unit);
    if (info == 0) {
        return 0;
    }
    return func_08A82B8C(&info->data, 4, 1);
}
