/* Releases every item of a pointer list through the global manager. */
extern void *D_089D7830;
extern void func_088CF970(void *mgr, int item);
void func_08A51F3C(int **list) {
    int i;
    int off;
    if (*list == 0) return;
    i = 0;
    off = 0;
    if (i < ((int *)list)[3]) {
        do {
            func_088CF970(D_089D7830, *(int *)((char *)*list + off));
            i++;
            off += 4;
        } while (i < ((int *)list)[3]);
    }
}
