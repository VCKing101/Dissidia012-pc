extern char *D_08A3CCF4;
extern char *D_08A3CCF8;
extern int D_08A3CCFC;

void func_08A3BF70(char *buffer, int size) {
    if (size > 0) {
        D_08A3CCF4 = buffer;
        D_08A3CCF8 = buffer + 4;
        D_08A3CCFC = 0;
    }
}
