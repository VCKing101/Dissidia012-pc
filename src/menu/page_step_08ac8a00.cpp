/* Runs the current step of a 4-step member-function table; true once the step is 4. */
class StepPageState {
public:
    char pad_000[0xBFC];
    int field_BFC;
};

class StepPage {
public:
    char pad_000[0xC18];
    StepPageState *field_C18;
    virtual void v0();
};

typedef void (StepPage::*StepPageStep)();
struct StepPageTable {
    StepPageStep steps[4];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const StepPageTable D_08ACA510;

extern "C" {
int func_08AC8A00(StepPage *self)
{
    StepPageTable table;
    func_08935524(&table, &D_08ACA510, sizeof(table));
    (self->*table.steps[self->field_C18->field_BFC])();
    return self->field_C18->field_BFC == 4;
}
}
