#include "menu/agent36_types.h"

/* Unlock list screen: writes the script commands of row (a mark when the
 * row has flag 0x80 or 0x40, then its text); returns how many. */

extern unsigned short D_08A2E722[];

int func_08A2A2D0(UnlockScreen36 *self, int row)
{
    int n = 0;
    ScriptCmd36 *cmd;

    if (self->work->flags[row] & 0x80) {
        cmd = &self->work->cmds[row][n];
        cmd->op = row + 0x23;
        cmd->arg1 = 0x2A;
        cmd->arg2 = row + 0x31;
        cmd->arg3 = 2;
        n++;
    } else if (self->work->flags[row] & 0x40) {
        cmd = &self->work->cmds[row][n];
        cmd->op = row + 0x1C;
        cmd->arg1 = 3;
        cmd->arg2 = row + 0x31;
        n++;
        cmd->arg3 = 1;
    }
    cmd = &self->work->cmds[row][n];
    cmd->op = row + 0x31;
    cmd->arg1 = 0x16;
    cmd->arg2 = 0x38;
    cmd->arg3 = D_08A2E722[row];
    return n + 1;
}
