/* Clears every triple of every grid row to 0xFF. */
#include "main/agent17_types.h"
extern "C" {
void func_088B7538(Grid *grid)
{
    for (int r = 0; r < 3; r++) {
        for (int t = 0; t < 6; t++) {
            for (int i = 0; i < 3; i++) {
                grid->rows[r].triples[t][i] = 0xFF;
            }
        }
    }
}
}
