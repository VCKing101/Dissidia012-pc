typedef struct UnitBody { char pad_00[0xD4]; float speed; float target; } UnitBody;
typedef struct Unit { char pad_00[0x20]; UnitBody *body; } Unit;
void func_08923E80(Unit *unit, float target)
{
    unit->body->speed = 0.0f;
    unit->body->target = target;
}
