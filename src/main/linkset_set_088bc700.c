/* Sets link 00 of the set, or clears it when null. */
#include "main/agent17_types.h"

void func_088BC700(LinkSet *set, void *link)
{
    void *none = 0;
    set->link_00 = link != 0 ? link : none;
}
