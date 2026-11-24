/* Takes the next 16-byte slot of a ring at 0xB4 and passes its address (+0x34) and a1 to func_089A3868. */
extern void func_089A3868(void *, int);
void func_0898FF74(char *self, int a1) {
    int *cnt = (int *)(self + 0xB4);
    int n = *cnt;
    char *slot = self + n * 16;
    n = n + 1;
    *cnt = n;
    func_089A3868(slot + 0x34, a1);
}
