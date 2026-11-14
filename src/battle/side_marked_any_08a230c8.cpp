typedef struct UnitStatus {
    char pad_0[0x5];
    unsigned char flags;

    static unsigned char isMarked(UnitStatus *status)
    {
        unsigned int bit = ((unsigned char *)status)[5] & 1;
        return bit;
    }
} UnitStatus;

typedef struct StatusUnit {
    char pad_000[0x5B0];
    UnitStatus status;
} StatusUnit;

extern "C" {

extern StatusUnit *func_08A22C78(int side);

int func_08A230C8(void)
{
    int result = 0;
    for (int i = 0; i < 2; i++) {
        StatusUnit *unit = func_08A22C78(i);
        if (unit == 0) {
            continue;
        }
        if (!UnitStatus::isMarked(&unit->status)) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

}
