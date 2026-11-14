#include "menu/auto_region3.h"

/* ScreenAB7CFC: hands field_7CCC its own field_24 and field_7CC8. */
void func_08AAB658(ScreenAB7CFC *self)
{
    int value;

    value = func_08AB47BC(self->work->field_7CCC);
    func_08AB45CC(self->work->field_7CCC, 1, value, self->work->field_7CC8);
}
