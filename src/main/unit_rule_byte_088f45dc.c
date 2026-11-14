typedef struct UnitRules {
    unsigned char field_00;
    unsigned char kind;
} UnitRules;

extern UnitRules *func_088C50D4(void *unit);

unsigned char func_088F45DC(void *unit)
{
    return func_088C50D4(unit)->kind;
}
