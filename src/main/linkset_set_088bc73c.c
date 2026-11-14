/* Sets link 0C of the set, or clears it when null. */
#include "main/agent17_types.h"

void func_088BC73C(LinkSet *set, void *link)
{
    void *none = 0;
    set->link_0C = link != 0 ? link : none;
}
