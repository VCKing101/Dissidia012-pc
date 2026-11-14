#include "battle/auto_region5.h"

/* PartF6008: runs func_0887E718 and func_0887E684 on D_089D6998, then frees
 * the object at field_F64. */
void func_08AF6008(PartF6008 *self)
{
    func_0887E718(D_089D6998);
    func_0887E684(D_089D6998);
    func_08890A00(self->field_F64);
    self->field_F64 = 0;
}
