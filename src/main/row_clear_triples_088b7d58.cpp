/* Clears every triple of a grid row to 0xFF. */
#include "main/agent17_types.h"

extern "C" {

void func_088B7D58(GridRow *row)
{
    for (int t = 0; t < 6; t++) {
        for (int i = 0; i < 3; i++) {
            row->triples[t][i] = 0xFF;
        }
    }
}

}
