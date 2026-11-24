/* Returns entry i of the ushort table selected by index k. */
extern unsigned short *D_08AB24A8[];
unsigned short func_08AB15EC(int i, int k) {
    return *(unsigned short *)((char *)D_08AB24A8[k] + i * 2);
}
