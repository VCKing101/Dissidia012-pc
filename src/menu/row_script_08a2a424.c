#include "menu/agent36_types.h"

/* Unlock list screen: builds and plays the script of row: an end mark, the
 * commands of the row's kind (or of an empty row past the count), and an
 * end mark after them. */
void func_08A2A424(UnlockScreen36 *self, int row)
{
    int n;

    func_08A2A3A0(self, self->work->cmds[row]);
    n = 0;
    if (self->work->count - 1 < row) {
        n = func_08A2A044(self, row);
    } else {
        switch (self->work->flags[row] & 0x1F) {
        case 0:
            n = func_08A2A11C(self, row);
            break;
        case 1:
            n = func_08A2A288(self, row);
            break;
        case 2:
            n = func_08A2A2D0(self, row);
            break;
        case 3:
            n = func_08A2A1EC(self, row);
            break;
        case 4:
            n = func_08A2A04C(self, row);
            break;
        }
    }
    func_08A2A3A0(self, &self->work->cmds[row][n]);
    func_088D3CF0(self->work->layout, self->work->handles, self->work->cmds[row]);
}
