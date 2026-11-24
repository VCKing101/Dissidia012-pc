/* Returns the short from func_088470DC(a, b, flag), or from func_088470DC(a, 1, flag) when that is null; -1 when a <= 0 or both are null. */
extern short *func_088470DC(int a, int b, unsigned char flag);
int func_088FB934(int a, int b, unsigned char flag)
{
    int result = -1;
    if (a > 0) {
        short *p = func_088470DC(a, b, flag);
        if (p != 0) {
            result = *p;
        } else {
            short *q = func_088470DC(a, 1, flag);
            if (q != 0) {
                result = *q;
            }
        }
    }
    return result;
}
