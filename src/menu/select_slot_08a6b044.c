/* Selects entry param_2: stores it, activates the table-selected object, then runs two follow-ups with the entry's id (or param_3 when given) and sets state 2. */
extern unsigned short D_08A6B358[];
extern void func_088D9F5C(int obj, int a, int b);
extern void func_08A80F14(int obj, int a);
extern void func_08A81240(int obj, int a);
void func_08A6B044(int self, int idx, int id) {
    int v;
    *(int *)(self + 0x674) = idx;
    func_088D9F5C(*(int *)(self + D_08A6B358[idx * 5 + 1] * 4 + 0x5B8), 4, 1);
    v = *(short *)(*(int *)(self + 0x66C) + idx * 0x2DC);
    if (id != -1) {
        v = id;
    }
    func_08A80F14(*(int *)(self + 0x434), 1);
    func_08A81240(*(int *)(self + 0x434), v);
    *(int *)(self + 0x28) = 2;
}
