/* Runs the current step of a four-step member-function table and reports whether the state reached 4. */
class StateRunInfo {
public:
    char pad_000[0xD98];
    int field_D98;
};

class StateRunScreen {
public:
    char pad_000[0xF64];
    StateRunInfo *field_F64;
    virtual void v0();
};

typedef void (StateRunScreen::*StateRunStep)();
struct StateRunTable {
    StateRunStep steps[4];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const StateRunTable D_08A39058;

extern "C" {
int func_08A379E4(StateRunScreen *self)
{
    StateRunTable table;
    func_08935524(&table, &D_08A39058, sizeof(table));
    (self->*table.steps[self->field_F64->field_D98])();
    int result = 0;
    if (self->field_F64->field_D98 == 4) {
        result = 1;
    }
    return result;
}
}
