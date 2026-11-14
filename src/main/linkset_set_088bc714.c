/* Sets link 04 of the set, or clears it when null. */
#include "main/agent17_types.h"

void func_088BC714(LinkSet *set, void *link)
{
    void *none = 0;
    set->link_04 = link != 0 ? link : none;
}
