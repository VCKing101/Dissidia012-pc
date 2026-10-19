/* Uncredited source probe; branch selection and narrowing do not match retail.
 * Kept separate from the configured main C units. */
#include "main/resource_descriptor.h"

void func_088CBA74(MainDescriptorPrefix *self, unsigned int index) {
    if (index < 11) {
        int value = D_089CBCC8[index];
        self->field_0002 = value < 0 ? 0 : 150 < value ? 150 : value;
    } else {
        self->field_0002 = 0;
    }
}
