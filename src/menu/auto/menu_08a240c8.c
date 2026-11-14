#include "menu/auto_region1.h"

/* Frees object from the menu root's heap (the screens' operator delete). */
void func_08A240C8(void *object)
{
    func_0885C538(D_08A23FF0->heap, object, 0);
}
