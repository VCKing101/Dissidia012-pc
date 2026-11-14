struct SpeedParams {
    char pad_000[0xC];
    float field_0C;
};

struct SpeedValue {
    float value;

    static float get(const SpeedValue &speed)
    {
        return speed.value;
    }
};

struct SpeedUnit {
    char pad_000[0x2C8];
    SpeedValue field_2C8;
};

struct SpeedOwner {
    char pad_000[0x270];
    SpeedUnit *unit;
};

extern "C" {

extern SpeedParams *func_08835134(SpeedUnit *unit, int a, int b);

float func_08A3EB2C(SpeedOwner *self)
{
    SpeedParams *params = func_08835134(self->unit, 1, 0);
    return params->field_0C * SpeedValue::get(self->unit->field_2C8);
}

}
