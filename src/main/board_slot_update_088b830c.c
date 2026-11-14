/* Copies the record's value into the board slot for the id and flags it as updated. */
#include "main/agent17_types.h"

typedef struct Record088B830C {
    short id;
    short fields[8];
    short value;
} Record088B830C;

Record088B830C *func_088B1D9C(int id);
BoardSlot *func_088B8414(Board *board, int id, int flag);

void func_088B830C(Board *board, int id)
{
    Record088B830C *rec = func_088B1D9C(id);
    if (rec != 0) {
        BoardSlot *slot = func_088B8414(board, id, 1);
        if (slot != 0) {
            slot->value = rec->value;
            slot->flags |= 2;
        }
    }
}
