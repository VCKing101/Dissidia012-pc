/* Descriptor initializer. Only the observed float fields have been typed;
 * the object's purpose and other fields remain unknown. */
#include "menu/configuration.h"

extern float D_08A93E20;
void func_08A93DF4(void) {
    float value = D_08A93E20 * 2.0f;
    D_08A94670.field_0020 = value;
    D_08A94670.field_0060 = value;
}
