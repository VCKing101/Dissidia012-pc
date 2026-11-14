#include "main/auto_region1.h"

/* Constructor: stores the value, then sets the object up with func_088118B8. */
Holder *func_0881173C(Holder *self, void *value)
{
    self->value = value;
    func_088118B8(self);
    return self;
}
