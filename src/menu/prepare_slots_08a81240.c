/* Queries the current id; when valid stores its remainder and quotient by 10 and two lookups in the owner and sets bit 4; returns whether it was valid. */
extern int func_08A7F2B4(void);
extern int func_08A7F104(int self, int a, int b);
int func_08A81240(int self) {
    int ok = 0;
    int id[2];
    id[0] = func_08A7F2B4();
    if (id[0] != -1) {
        int v;
        *(int *)(*(int *)(self + 0x28) + 0x608) = id[0] % 10;
        *(int *)(*(int *)(self + 0x28) + 0x60C) = id[0] / 10;
        v = func_08A7F104(self, 2, -1);
        *(int *)(*(int *)(self + 0x28) + 0x5FC) = v;
        v = func_08A7F104(self, 1, -1);
        *(int *)(*(int *)(self + 0x28) + 0x600) = v;
        ok = 1;
        *(int *)(*(int *)(self + 0x28) + 0x614) = *(int *)(*(int *)(self + 0x28) + 0x600);
        *(unsigned *)(*(int *)(self + 0x28) + 0x5E8) = *(unsigned *)(*(int *)(self + 0x28) + 0x5E8) | 4;
    }
    return ok;
}
