/* Reads two counted arrays from a data stream (see 0x08819AD0). */
extern "C" {

int func_08819B18(int *count_a, char **items_a, int *count_b, char **items_b, char **cursor, int size_a, int size_b)
{
    if (*cursor == 0) {
        return -1;
    }
    *count_a = *(int *)*cursor;
    *cursor += 4;
    *items_a = *cursor;
    *cursor += size_a * *count_a;
    *count_b = *(int *)*cursor;
    *cursor += 4;
    *items_b = *cursor;
    *cursor += size_b * *count_b;
    return 0;
}

}
