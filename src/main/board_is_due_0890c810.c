#include "main/agent21_types.h"
int func_0890C810(Board *board)
{
    int index = board->selected < board->itemCount ? board->selected : -1;
    if (index >= 0) {
        BoardItem *items = board->items;
        int due = items[index].time + 90;
        if (!(board->time < due)) {
            return 0;
        }
    }
    return 1;
}
