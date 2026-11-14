/* The pair's value when its key matches, else 0. */
int func_08AF4C0C(short *pair, int key)
{
    int value = 0;
    if (*pair == key) {
        pair += 2;
        value = *pair;
    }
    return value;
}
