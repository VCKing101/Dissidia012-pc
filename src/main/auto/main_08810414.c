#include "main/auto_region1.h"

/* Reads a loaded file's header: counts as they are, offsets turned into
 * addresses. */
void func_08810414(FileView810414 *view, FileHeader810414 *header)
{
    view->count_00 = header->count_04;
    view->table_04 = header->offset_08 + (int)header;
    view->count_08 = header->count_1C;
    view->table_0C = header->offset_20 + (int)header;
    view->table_10 = header->offset_10 + (int)header;
    view->table_14 = header->offset_18 + (int)header;
}
