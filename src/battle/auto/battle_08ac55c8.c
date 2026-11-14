#include "battle/auto_region4.h"

typedef struct OwnerC55C8 {
    char pad_000[0xD60];
    char field_D60;
} OwnerC55C8;

typedef struct TableC55C8 {
    int entries[4];
    OwnerC55C8 *owner;
} TableC55C8;

/* The entry chosen by the owner's byte at field_D60. */
int func_08AC55C8(TableC55C8 *self)
{
    return self->entries[self->owner->field_D60];
}
