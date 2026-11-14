extern int D_089D4978[16][16];
extern void func_08935524(void *dst, const void *src, int size);

int func_0884A454(int row, int col)
{
    int table[16][16];

    func_08935524(table, D_089D4978, 0x400);
    return table[row][col];
}
