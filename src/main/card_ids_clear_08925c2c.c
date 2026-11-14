typedef struct CardBody { char pad_00[0x8C]; short ids[21]; signed char count; } CardBody;
typedef struct Unit { char pad_00[0x20]; CardBody *body; } Unit;
void func_08925C2C(Unit *unit)
{
    int i;
    for (i = 0; i < 21; i++) {
        unit->body->ids[i] = -1;
    }
    unit->body->count = 0;
}
