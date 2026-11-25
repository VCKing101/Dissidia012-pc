/* Forwards the second argument, the word at 0x14C of the first, and the last two arguments to func_08931C28. */
extern void func_08931C28(int, int, int, int);
void func_0892F7D0(int *self, int a, int b, int c) {
    int t = self[0x14C / 4];
    func_08931C28(a, t, b, c);
}
