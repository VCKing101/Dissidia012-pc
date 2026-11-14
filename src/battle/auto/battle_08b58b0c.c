#include "battle/auto_region5.h"

/* Part58A9C: sets up the drawing state, draws the handle and, when there
 * is one, the ten entries. */
void func_08B58B0C(Part58A9C *self)
{
    int i;

    func_089A296C(2);
    func_089A250C();
    func_089A2568();
    func_0899FDC4(0);
    func_089A0384(1);
    func_089A0384(4);
    func_089A0384(9);
    func_089A0384(0x15);
    func_089A119C(1);
    func_089A0C8C(-1);
    func_0899D2F8(self->field_830);
    func_089A12A4(0, 2, 10, 0, 0xFFFFFFFF);
    if (self->field_830 != 0) {
        for (i = 0; i < 10; i++) {
            func_08B598AC(self->entries[i]);
        }
    }
}
