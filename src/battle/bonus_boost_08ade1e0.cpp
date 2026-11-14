typedef struct BonusInfo {
    short pad_00;
    short bonus;
} BonusInfo;

typedef struct BoostStatus {
    int pad_00;
    short percent;
} BoostStatus;

extern "C" {

extern void *func_08835798(void *unit);
extern BoostStatus *func_088BF4CC(void *status, int id);

/* base plus the info's bonus, raised by the unit's status 0x104 percentage. */
int func_08ADE1E0(void *self, BonusInfo *info, void *unit, int base)
{
    if (info != 0) {
        int value = info->bonus + base;
        BoostStatus *boost = func_088BF4CC(func_08835798(unit), 0x104);
        if (boost != 0) {
            short *percent = &boost->percent;
            value = value * (*percent + 100) / 100;
        }
        return value;
    }
    return 0;
}

}
