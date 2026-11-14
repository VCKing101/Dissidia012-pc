typedef struct History {
    char pad_00[0x14];
    int pos;                    /* 0x14 */
    char pad_18[8];
    signed char count;          /* 0x20 */
} History;

int func_0890D550(History *history, int value)
{
    if (!(history->pos < value) && history->pos - history->count < value) {
        return 1;
    }
    return 0;
}
