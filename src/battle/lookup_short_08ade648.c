/* Returns the short found for the object's id at +0x22 in the table, or -1. */
extern void *D_089D6C30;
extern short *func_0889F3E4(void *table, int id);

int func_08ADE648(int unused, char *obj)
{
    int neg = -1;
    short *s;
    if (obj != 0 && obj[0x22] != neg) {
        s = func_0889F3E4(D_089D6C30, obj[0x22]);
        if (s != 0) {
            return *s;
        }
    }
    return neg;
}
