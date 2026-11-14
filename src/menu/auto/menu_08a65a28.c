#include "menu/auto_region2.h"

/* Page680D8: copies 0x10 bytes into its entry. */
void func_08A65A28(Page680D8 *self, const void *src)
{
    func_08935524(self->entry, src, 0x10);
}
