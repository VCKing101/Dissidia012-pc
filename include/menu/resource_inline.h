#ifndef MENU_RESOURCE_INLINE_H
#define MENU_RESOURCE_INLINE_H

#include "menu/resource_phases.h"

/* A partial flag accessor, not a claim about the full resource class. SNC
 * preserves the bool return conversion when inlining this observed query. */
struct MenuResourceFlags {
    static bool has_0020(const MenuResourceView *resource) {
        return (resource->field_0020 & 0x20) != 0;
    }
};

#endif
