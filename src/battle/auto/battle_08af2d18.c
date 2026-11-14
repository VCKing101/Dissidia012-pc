#include "battle/auto_region4.h"

typedef struct SourceF2D18 {
    char pad_00[8];
    int field_08;
    char pad_0C[0x10];
    int field_1C;
} SourceF2D18;

/* PartF2CBC: hands field_08 and field_1C of `source` to the members at
 * field_04 and field_18. */
void func_08AF2D18(PartF2CBC *self, SourceF2D18 *source)
{
    func_088EC500(self->field_04, source->field_08);
    func_088EC2A8(self->field_18, source->field_1C);
}
