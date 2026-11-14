#include "menu/auto_region1.h"

/* ScreenE9D38: keeps value in field_148 (clearing field_178 unless keep),
 * then starts its task with the callbacks func_089E93A0 / func_089E93C0. */
void func_089E7DF0(ScreenE9D38 *self, unsigned int value, unsigned char keep)
{
    MenuTask *task;

    self->field_148 = value;
    if (keep == 0) {
        self->field_178 = 0;
    }
    task = &self->task;
    func_0886503C(task, func_089E93A0, func_089E93C0, 0);
    func_0886504C(task, self, 0x100);
    func_088650A4(task);
}
