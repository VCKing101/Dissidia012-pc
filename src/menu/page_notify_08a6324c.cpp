#include "menu/agent6_types.h"

struct NotifyPage {
    char pad_000[0xD14];
    A6Part *field_D14[7];
};

extern "C" {
void func_08A6324C(NotifyPage *self)
{
    for (int i = 0; i < 7; i++) {
        self->field_D14[i]->v5();
    }
}
}
