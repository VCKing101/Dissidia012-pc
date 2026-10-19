/* Uncredited probe: repeated flag loads, store positions and branch forms
 * differ under the pinned compiler profile. */
#include "main/drawing_record.h"

void func_088D9D24(MainDrawingRecordPrefix *self) {
    if (self->field_000C != -1 && self->field_0028[3] &&
        (unsigned char)((self->field_0020 & 8) != 0) &&
        !(unsigned char)((self->field_0020 & 0x4000) != 0)) {
        self->field_0020 |= 0x8000;
    } else self->field_0020 &= ~0x8000;
    if (self->field_000C != -1 && (unsigned char)((self->field_0020 & 8) != 0) &&
        !(unsigned char)((self->field_0020 & 0x20) != 0)) {
        self->field_0020 |= 1;
    } else self->field_0020 &= ~1;
}
