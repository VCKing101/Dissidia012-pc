/* Runs the current step of a four-step member-function table; afterwards reports 4 (or 2 with the flag set) once the state reached 4, else 0. */
class StateRunProgress {
public:
    char pad_000[0x3E8];
    int field_3E8;
    unsigned char field_3EC;
};

class StateRunPanel {
public:
    char pad_000[0x418];
    StateRunProgress *field_418;
    virtual void v0();
};

typedef void (StateRunPanel::*StateRunPanelStep)();
struct StateRunPanelTable {
    StateRunPanelStep steps[4];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const StateRunPanelTable D_08A48288;

extern "C" {
int func_08A47290(StateRunPanel *self)
{
    StateRunPanelTable table;
    func_08935524(&table, &D_08A48288, sizeof(table));
    (self->*table.steps[self->field_418->field_3E8])();
    int result = 0;
    if (self->field_418->field_3E8 == 4) {
        result = self->field_418->field_3EC ? 2 : 4;
    }
    return result;
}
}
