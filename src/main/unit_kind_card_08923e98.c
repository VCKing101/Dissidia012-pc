typedef struct UnitBody { char pad_00[0x12]; short kind; } UnitBody;
typedef struct Unit { char pad_00[0x20]; UnitBody *body; } Unit;
int func_08923E98(Unit *unit)
{
    switch (unit->body->kind) {
    case 3:
    case 5:
        return 1;
    }
    return 0;
}
