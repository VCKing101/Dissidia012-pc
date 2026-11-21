/* libGu: the current list's cursor, and (if asked) the bytes left in the list after it. */
#include "main/agent34_sdk.h"

u32 *func_089A0144(int *left)
{
    if (left != 0) {
        u32 *start = D_09AF67E8.context->list_start;
        *left = D_09AF67E8.context->list_size - ((char *)D_09AF67E8.context->list_cursor - (char *)start);
    }
    return D_09AF67E8.context->list_cursor;
}
