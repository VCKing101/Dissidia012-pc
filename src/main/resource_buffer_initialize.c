/* Observed resource-buffer fields only; complete object ownership is unknown. */
typedef struct MainResourceBufferPrefix {
    unsigned char unknown_0000[4];
    unsigned int field_0004;
    void *field_0008;
    unsigned int field_000C;
} MainResourceBufferPrefix;

/* Native entry selects the heap default when alignment <= 0, narrows the
 * last two flags to bytes, and overwrites the unused fourth argument. */
#include "main/allocator_runtime.h"
extern void *D_089D6C88;
extern void func_088A18B0(void *, int, int);
extern unsigned int D_09B69878[2];

void func_088D52AC(void *object, void *allocator, unsigned int size) {
    MainResourceBufferPrefix *self = object;
    self->field_0004 = 0;
    self->field_0008 = func_0885C6CC(allocator,size,0x80,0,1,0);
    self->field_000C = 0;
    func_088A18B0(D_089D6C88,100,0);
    D_09B69878[0] = 0;
    D_09B69878[1] = 0;
}
