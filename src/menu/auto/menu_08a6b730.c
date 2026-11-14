#include "menu/auto_region2.h"

/* Panel6C638: the selected record's field_10. */
char *func_08A6B730(Panel6C638 *self)
{
    return (*self->data)->records[self->cursor.index].field_10;
}
