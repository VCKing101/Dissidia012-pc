typedef struct Info978C {
    char pad_00[0x14];
    short field_14;
} Info978C;

typedef struct Unit978C {
    char pad_000[0x52C];
    Info978C *field_52C;
} Unit978C;

extern Unit978C *func_089E95B0(int id);
extern int func_08AE3C70(int id);

int func_089E978C(int id)
{
    if ((unsigned int)id >= 3) {
        return 0;
    }
    if (id >= 0 && id < 2) {
        Unit978C *unit = func_089E95B0(id);
        if (unit == 0) {
            return 0;
        }
        return unit->field_52C->field_14;
    }
    return func_08AE3C70(id);
}
