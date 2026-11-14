#include "menu/agent6_types.h"

extern "C" {
void func_08A54E0C(void);
void func_08A54C60(A6Big *self);

void func_08A54890(A6Big *self) {
    func_08A54E0C();
    func_08A54C60(self);
    self->s14();
}
}
