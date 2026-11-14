/* Sets link 10 of the set, or clears it when null. */
#include "main/agent17_types.h"

void func_088BC750(LinkSet *set, void *link)
{
    void *none = 0;
    set->link_10 = link != 0 ? link : none;
}
