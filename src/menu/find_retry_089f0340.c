/* Looks an entry up, then again with the fallback flag; -1 becomes 0. */
extern int func_089F0274(int table, int key, int extra, int fallback);

int func_089F0340(int table, int key, int extra)
{
    int found = func_089F0274(table, key, extra, 0);
    if (found == -1) {
        found = func_089F0274(table, key, extra, 1);
    }
    if (found == -1) {
        found = 0;
    }
    return found;
}
