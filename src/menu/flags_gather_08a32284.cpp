extern "C" {
extern int func_08A3200C(void);
extern int func_088470DC(int actor, int index, int flag);
}

extern "C" unsigned int func_08A32284(void)
{
    unsigned int mask = 0;
    int actor = func_08A3200C();
    int i;
    for (i = 4; i < 13; i++) {
        if (func_088470DC(actor, i, 0) != 0) {
            mask |= 1 << i;
        }
    }
    return mask;
}
