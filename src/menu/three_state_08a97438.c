/* Returns 0 for the none id (-1), 1 when its bit is set in the global bitset, otherwise -1. */
extern unsigned int *D_089D726C;
int func_08A97438(int unused, unsigned int id) {
    int none = -1;
    int set;
    if (id == none) {
        return 0;
    }
    set = (D_089D726C[id >> 5] & (1 << (id & 0x1F))) != 0;
    if (set == 1) {
        return 1;
    }
    return none;
}
