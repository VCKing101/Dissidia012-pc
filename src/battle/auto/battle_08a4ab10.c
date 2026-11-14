#include "battle/auto_region3.h"

typedef struct Buffer4AB10 {
    char pad_00[0x14];
    void *field_14;
} Buffer4AB10;

/* Keeps the 0x100-byte buffer in field_14 and clears it. */
void func_08A4AB10(Buffer4AB10 *self, void *buffer)
{
    self->field_14 = buffer;
    func_08935828(buffer, 0, 0x100);
}
