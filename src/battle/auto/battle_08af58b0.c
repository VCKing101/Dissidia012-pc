#include "battle/auto_region5.h"

/* BufferF58B0 constructor: clears the counters and the three buffers. */
BufferF58B0 *func_08AF58B0(BufferF58B0 *self)
{
    self->field_14 = 0;
    self->field_818 = 0;
    self->field_81C = 0;
    func_08935828(self->field_00, 0, sizeof(self->field_00));
    func_08935828(self->field_18, 0, sizeof(self->field_18));
    func_08935828(self->field_418, 0, sizeof(self->field_418));
    return self;
}
