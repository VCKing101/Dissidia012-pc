typedef struct Histories {
    char pad_00[0xCC];
    int pos;                    /* 0xCC */
    char pad_D0[8];
    signed char count;          /* 0xD8 */
} Histories;

int func_0890D588(Histories *history, int back)
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
