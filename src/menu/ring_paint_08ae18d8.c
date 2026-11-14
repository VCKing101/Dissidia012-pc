extern void func_08AE1860(void *self, unsigned char flag, int index, int layer, int frame);

void func_08AE18D8(void *self, unsigned char flag)
{
    int i;
    for (i = 0; i < 4; i++) {
        func_08AE1860(self, flag, i, 0, i);
        func_08AE1860(self, flag, i, 1, i % 2 + 4);
    }
}
