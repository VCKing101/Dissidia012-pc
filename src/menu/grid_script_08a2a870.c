#include "menu/agent36_types.h"

/* Grid page: builds and plays the script of row: an end mark, then for a
 * row in use its text and the commands of its kind, and an end mark after
 * them. */
void func_08A2A870(GridScreen36 *self, void *arg, int row)
{
    int n;

    func_08A2A858(self, self->work->cells[row]);
    if (!(self->work->count - 1 < row)) {
        func_08A2A9E4(self, arg, row);
        n = 0;
        switch (self->work->flags[row] & 0x1F) {
        case 0:
            n = func_08A2A6D8(self, row);
            break;
        case 1:
            n = func_08A2A798(self, row);
            break;
        case 2:
            n = func_08A2A7E4(self, row);
            break;
        case 3:
            n = func_08A2A74C(self, row);
            break;
        case 4:
            n = func_08A2A614(self, row);
            break;
        }
        func_08A2A858(self, &self->work->cells[row][n]);
    }
    func_088D428C(self->work->handles, self->work->texts, self->work->cells[row], 1);
}
