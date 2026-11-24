/* When the global query returns 1, sets bit 2 of the flags short of every entry (stride 8) the second query does not return 1 for. */
extern int D_089D8140;
extern int func_08900E60(int g);
extern int func_08900F50(int g, int id);
void func_08A871D4(int unused, unsigned short *entries, int count) {
    int i;
    if (func_08900E60(D_089D8140) == 1) {
        i = 0;
        if (i < count) {
            do {
                int id = *(int *)entries;
                if (func_08900F50(D_089D8140, id) != 1) {
                    entries[2] = entries[2] | 2;
                }
                i = i + 1;
                entries = entries + 4;
            } while (i < count);
        }
    }
}
