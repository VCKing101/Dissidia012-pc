typedef struct History {
    char pad_00[0x14];
    int pos;                    /* 0x14 */
    char pad_18[8];
    signed char count;          /* 0x20 */
} History;

int func_0890D524(History *history, int back)
{
    int result = -1;
    if (history->count > 0) {
        int index = history->pos - back;
        if (index < history->count) {
            result = index;
        }
    }
    return result;
}
