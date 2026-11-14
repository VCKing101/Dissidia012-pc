typedef struct Info9C24 { char pad_000[0x28]; unsigned int field_28; } Info9C24;
typedef struct Unit9C24 { char pad_000[0x52C]; Info9C24 *field_52C; } Unit9C24;
extern Unit9C24 *func_089E95B0(unsigned int);

int func_089E9C24(unsigned int index)
{
    Unit9C24 *unit = func_089E95B0(index);
    if (!unit) {
        return 0;
    }
    return (unit->field_52C->field_28 & 4) != 0;
}
