extern int D_08AE7CA4;
extern int D_09B6C160[17][4];

void func_08AE3BC8(void)
{
    int i;
    int j;
    D_08AE7CA4 = 0;
    for (i = 0; i < 17; i++) {
        for (j = 0; j < 4; j++) {
            D_09B6C160[i][j] = 0;
        }
    }
}
