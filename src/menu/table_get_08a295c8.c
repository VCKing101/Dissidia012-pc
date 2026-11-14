extern int D_08A29BF0;
extern int D_09B6D9F8[];

int func_08A295C8(int index)
{
    if (D_08A29BF0 != 0) {
        if (index < 0 || index >= 5) {
            index = 0;
        }
        return D_09B6D9F8[index];
    }
    return 0;
}
