/* Runs the current step of a 10-step member-function table; true once the step is 10. */
class StepBattleState {
public:
    char pad_000[0x6A8];
    int field_6A8;
};

class StepBattle {
public:
    char pad_000[0x6DC];
    StepBattleState *field_6DC;
    virtual void v0();
};

typedef void (StepBattle::*StepBattleStep)();
struct StepBattleTable {
    StepBattleStep steps[10];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const StepBattleTable D_08A35A04;

extern "C" {
int func_08A30CC0(StepBattle *self)
{
    StepBattleTable table;
    func_08935524(&table, &D_08A35A04, sizeof(table));
    (self->*table.steps[self->field_6DC->field_6A8])();
    return self->field_6DC->field_6A8 == 10;
}
}
