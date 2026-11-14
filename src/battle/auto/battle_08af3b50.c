#include "battle/auto_region4.h"

typedef struct HolderF3B50 {
    char pad_00[4];
    void *field_04;
} HolderF3B50;

/* Calls func_08AF3994 on the object at field_04. */
void func_08AF3994(void *object);

void func_08AF3B50(HolderF3B50 *self)
{
    func_08AF3994(self->field_04);
}
