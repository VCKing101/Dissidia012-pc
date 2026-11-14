/* Reads one cell of the two-column table at 0x089CB538. */
extern int D_089CB538[][2];

int func_088AFB8C(int row, int column)
{
    return D_089CB538[row][column];
}
