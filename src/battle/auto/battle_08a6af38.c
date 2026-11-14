#include "battle/auto_region3.h"

typedef struct Holder6AF38 {
    char pad_000[0x39C];
    int field_39C;
} Holder6AF38;

typedef struct Result6AF38 {
    char pad_00[4];
    Holder6AF38 *field_04;
} Result6AF38;

typedef struct Owner6AF38 {
    char pad_00[0x64];
    void *field_64;
} Owner6AF38;

/* field_39C of the holder that func_08AB3264 finds for field_64, or 0
 * without one. */
int func_08A6AF38(Owner6AF38 *self)
{
    Result6AF38 *result;

    result = func_08AB3264(self->field_64);
    if (result->field_04 == 0) {
        return 0;
    } else {
        return result->field_04->field_39C;
    }
}
