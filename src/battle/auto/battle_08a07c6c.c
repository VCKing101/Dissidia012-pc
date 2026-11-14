#include "battle/auto_region2.h"

/* Steps field_5C, first telling it which way field_48 reports when field_5C is ready. */
int func_08A07C6C(Part08158 *self)
{
    PartF5E58 *part = &self->field_5C;

    if (func_089F5C54(part) == 1) {
        if (func_089F1298(&self->field_48) == 1) {
            func_089F5B5C(part);
        } else {
            if (func_089F1274(&self->field_48) == 1) {
                func_089F5B84(part);
            }
        }
    }
    func_089F58FC(part);
    return 0;
}
