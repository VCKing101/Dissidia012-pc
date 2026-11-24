/* Returns 1 when the first check fails to flag the id and the second check passes, otherwise 0. */
extern int func_08AB9B84(int self, int id);
extern int func_088F45DC(int v);
extern int func_08AB8128(int self, int id);
int func_08AB8BC0(int self) {
    int id = *(int *)(*(int *)(self + 0x138) + 0x8B0);
    if (func_088F45DC(func_08AB9B84(self, id)) != 0) {
        return 0;
    }
    if (func_08AB8128(self, id) == 0) {
        return 0;
    }
    return 1;
}
