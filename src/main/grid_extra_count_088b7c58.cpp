/* Number of extra cells that hold a value. */
#include "main/agent17_types.h"

extern "C" {

int func_088B7AE8(Grid *grid, int i);

int func_088B7C58(Grid *grid)
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (func_088B7AE8(grid, i) != -1) {
            count++;
        }
    }
    return count;
}

}
