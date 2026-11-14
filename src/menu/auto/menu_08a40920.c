#include "menu/auto_region2.h"

/* Frees a screen from the second menu root's heap. */
void func_08A40920(void *object)
{
    func_0885C538(((MenuRoot2 *)D_08A457EC)->heap, object, 0);
}
