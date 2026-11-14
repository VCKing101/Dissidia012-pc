#include "menu/auto_region1.h"

/* Screen0B6A0: runs its set-up steps and keeps field_3258 - 1 and
 * field_326C - 1 in field_325A and field_326E. */
void func_089F9A78(Screen0B6A0 *self)
{
    func_089F8B1C(self);
    func_089F8CC8(self);
    func_08A03D48(self);
    func_08A04164(self);
    func_089F91F0(self);
    func_08A043E4(self);
    func_089F9388(self);
    func_089F9674(self);
    func_089F9954(self);
    self->work->field_325A = self->work->field_3258 - 1;
    self->work->field_326E = self->work->field_326C - 1;
    func_089F9C08(self);
}
