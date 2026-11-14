typedef struct CardBody { char pad_00[0x8C]; short ids[21]; signed char count; } CardBody;
typedef struct Unit { char pad_00[0x20]; CardBody *body; } Unit;
int func_08925C60(Unit *unit, short id)
{
    int n = unit->body->count;
    unit->body->ids[n] = id;
    unit->body->count++;
    return n;
}
