/* Runs the current step of the six-step member-function table and reports whether the screen finished (10000). */
class UpdateScreenBase {
public:
    virtual void slot_08();
};

struct UpdateState {
    int field_000;
    char pad_004[0x5E4 - 0x4];
    int field_5E4;
    char pad_5E8[0x5F8 - 0x5E8];
    int field_5F8;
};

class UpdateScreen : public UpdateScreenBase {
public:
    char pad_004[0x28 - 0x4];
    UpdateState *field_028;
};

typedef void (UpdateScreen::*UpdateScreenStep)();
struct UpdateScreenTable {
    UpdateScreenStep steps[6];
};

extern "C" {
void func_08935524(void *dst, const void *src, int size);
void func_08A80BD4(UpdateScreen *self);
void func_08A84458(UpdateScreen *self);
void func_08A7FA7C(UpdateScreen *self);
extern const UpdateScreenTable D_08A85E38;

int func_08A80E34(UpdateScreen *self)
{
    UpdateScreenTable table;
    self->field_028->field_5F8 = 0;
    func_08A80BD4(self);
    func_08935524(&table, &D_08A85E38, sizeof(table));
    (self->*table.steps[self->field_028->field_000])();
    func_08A84458(self);
    func_08A7FA7C(self);
    if (self->field_028->field_5E4 == 10000) {
        self->field_028->field_5F8 = 1;
    }
    return self->field_028->field_5F8;
}
}
