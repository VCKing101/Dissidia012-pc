typedef struct Info973C { char pad_000[0xC]; int field_0C; char pad_010[4]; short field_14; } Info973C;
typedef struct Unit973C { char pad_000[0x52C]; Info973C *field_52C; } Unit973C;
extern Unit973C *func_089E95B0(unsigned int);
extern int func_089E99C0(unsigned int, int);

int func_089E9964(unsigned int index)
{
    Unit973C *unit;
    if (index >= 2) {
        return 0;
    }
    unit = func_089E95B0(index);
    if (!unit) {
        return 0;
    }
    return func_089E99C0(index, unit->field_52C->field_14);
}
