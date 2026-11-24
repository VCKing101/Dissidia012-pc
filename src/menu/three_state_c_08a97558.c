/* Looks up a record, takes its id (field +4 or +6 by flag), and returns 0 for -1, 1 when its bit is set in the global bitset, else -1. */
extern unsigned int *D_089D726C;
extern int func_088C5B34(int a);
int func_08A97558(int unused, int a, int flag) {
    int rec = func_088C5B34(a);
    int none = -1;
    unsigned int id;
    int set;
    if (flag == 0) {
        id = *(short *)(rec + 4);
    } else {
        id = *(short *)(rec + 6);
    }
    if (id == none) {
        return 0;
    }
    set = (D_089D726C[id >> 5] & (1 << (id & 0x1F))) != 0;
    if (set == 1) {
        return 1;
    }
    return none;
}
