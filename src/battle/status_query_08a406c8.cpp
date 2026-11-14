struct StatusParam406C8 {
    short value;
};

struct StatusEntry406C8 {
    char pad_000[0x4];
    StatusParam406C8 param;
};

extern "C" {
extern int func_08B2F3B0(void *unit);
extern void *func_08835778(void *unit);
extern StatusEntry406C8 *func_088BF4CC(void *table, int id);
extern int func_08A642D8(int id);
}

struct StatusUnit406C8 {
    char pad_000[0x5C4];
    int field_5C4;

    static StatusParam406C8 *find_param(StatusUnit406C8 *unit, int id)
    {
        return &func_088BF4CC(func_08835778(unit), id)->param;
    }

    static bool has_status(StatusUnit406C8 *unit, int id)
    {
        return func_088BF4CC(func_08835778(unit), id) != 0;
    }
};

struct StatusOwner406C8 {
    StatusUnit406C8 *unit;
};

extern "C" {

int func_08A406C8(StatusOwner406C8 *self)
{
    if (func_08B2F3B0(self->unit)) {
        return 1;
    }
    if (StatusUnit406C8::has_status(self->unit, 0x134)) {
        return 1;
    }
    return 0;
}

int func_08A40730(StatusOwner406C8 *self)
{
    if (func_08B2F3B0(self->unit)) {
        return func_08A642D8(self->unit->field_5C4);
    }
    if (StatusUnit406C8::has_status(self->unit, 0x134)) {
        return StatusUnit406C8::find_param(self->unit, 0x134)->value;
    }
    return 0;
}

}
