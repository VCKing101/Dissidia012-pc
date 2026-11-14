typedef struct Histories {
    char pad_00[0xEC];
    int count;                  /* 0xEC */
    int pos;                    /* 0xF0 */
} Histories;

int func_0890D638(Histories *history, int back)
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
