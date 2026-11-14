/* Reads a counted array from a data stream: the count word, then `size`
   bytes per element; advances the cursor past it. */
extern "C" {

int func_08819AD0(int *count, char **items, char **cursor, int size)
{
    if (*cursor == 0) {
        return -1;
    }
    *count = *(int *)*cursor;
    *cursor += 4;
    *items = *cursor;
    *cursor += size * *count;
    return 0;
}

}
