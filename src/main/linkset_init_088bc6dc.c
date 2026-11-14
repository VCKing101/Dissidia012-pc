/* Clears a link set and returns it. */
#include "main/agent17_types.h"

LinkSet *func_088BC6DC(LinkSet *set)
{
    set->link_00 = 0;
    set->link_04 = 0;
    set->link_08 = 0;
    set->link_0C = 0;
    set->link_10 = 0;
    set->flag_14 = 0;
    set->flag_15 = 0;
    return set;
}
