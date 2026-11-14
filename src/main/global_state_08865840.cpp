#include "main/agent2_types.h"

extern "C" {
void func_0885E7C0(void *);
void func_08873498(void *);
void func_0885E834(void *, int);
void func_088734D4(void *, int);
Agent2Point *func_08864C1C(Agent2Point *);
void func_08864C48(void *, unsigned int);
void func_08992F04(void *);
}

struct Agent2GlobalSetup {
    /* Default fade colour and no selected track. */
    static void reset_display(Agent2GlobalState *self) {
        self->field_08BC = 0xFF000080;
        self->field_08C0 = -1;
    }
};

extern "C" Agent2GlobalState *func_08865840(Agent2GlobalState *self) {
    self->field_0714 = 0;
    func_0885E7C0(self->field_0720);
    func_08873498(self->field_07E0);
    func_08864C1C(&self->field_08B0);
    Agent2GlobalSetup::reset_display(self);
    self->field_1AC4 = 0;
    self->field_2CC8 = 0;
    self->field_2CCC = 0;
    self->field_2CCE = 0;
    self->field_2CD1 = 0;
    self->field_2CCF = 0;
    self->field_2CD0 = 0;
    return self;
}

extern "C" void func_088658B0(Agent2GlobalState *self, unsigned int flags) {
    if (self) {
        func_08864C48(&self->field_08B0, 2);
        func_088734D4(self->field_07E0, 2);
        func_0885E834(self->field_0720, 2);
        if (flags & 1) func_08992F04(self);
    }
}
