struct Gauge96DC {
    char pad_00[0x8];
    int field_08;
    int field_0C;
};

struct Unit96DC {
    char pad_000[0x52C];
    Gauge96DC *field_52C;

    static int remaining(Unit96DC *unit)
    {
        int left = unit->field_52C->field_0C - unit->field_52C->field_08;
        if (left < 0) {
            left = 0;
        }
        return left;
    }
};

extern "C" {

extern Unit96DC *func_089E95B0(unsigned int index);

int func_089E96DC(unsigned int index)
{
    int result;
    if (index >= 2) {
        return 0;
    }
    {
        Unit96DC *unit;
        result = 0;
        unit = func_089E95B0(index);
        if (unit) {
            result = Unit96DC::remaining(unit);
        }
    }
    return result;
}

}
