/* Starts the matching sub-effect for one of three known ids, stores the id and runs the follow-up. */
extern void func_08A79EF0(int p, int owner, int a, int b, int c);
extern void func_08A79E68(int p, int a);
void func_08A6D1B0(int self, int id) {
    int p;
    int k = id;
    p = self + 0x1B0;
    switch (k) {
    case -0x7FBEF4FD:
        func_08A79EF0(p, self, 0xD, 0, 0);
        break;
    case -0x7FBEF4F7:
        func_08A79EF0(p, self, 0xE, 0, 0);
        break;
    case -0x7FBEF4F1:
        func_08A79EF0(p, self, 0xF, 0, 0);
        break;
    default:
        break;
    }
    *(int *)(self + 0x260) = id;
    func_08A79E68(p, 0xD);
}
