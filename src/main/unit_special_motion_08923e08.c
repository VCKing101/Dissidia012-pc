typedef struct UnitModel { char pad_00[0x130]; char motion[4]; } UnitModel;
typedef struct UnitBody { char pad_00[0xBC]; UnitModel *model; } UnitBody;
typedef struct Unit { char pad_00[0x20]; UnitBody *body; } Unit;
extern int func_089237AC(Unit *unit);
extern void func_08B347AC(UnitModel *model, float from, float to);
extern void func_0882FA10(void *motion, int kind, int flags, float rate, float start);
int func_08923E08(Unit *unit)
{
    if (func_089237AC(unit) != 0) {
        UnitModel *model = unit->body->model;
        if (model != 0) {
            func_08B347AC(model, 30.0f, 30.0f);
            func_0882FA10(unit->body->model->motion, 10000, 0, 0.0f, 0.0f);
            return 1;
        }
    }
    return 0;
}
