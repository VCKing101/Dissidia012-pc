/* Returns the position of `value` in the D_08B22DEC list (-1 terminated),
   or 0 when it is not there. */
extern int D_08B22DEC[];

int func_08B1E0E8(void *self, int value)
{
    int found = 0;
    int i = 0;
    int *entry = D_08B22DEC;
    for (;;) {
        if (*entry == -1) {
            break;
        }
        if (*entry == value) {
            found = i;
            break;
        }
        entry++;
        i++;
    }
    return found;
}
