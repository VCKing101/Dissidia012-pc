/* Stores (result != 1) in the global at 0x09B6EEF0, then runs the setup on self with the second argument. */
extern int D_09B6DF08[];
extern int func_08A7046C(void);
extern void func_08A71E44(int self, int b);
void func_08A720C4(int self, int b) {
    int s = self;
    int k = b;
    int *g = (int *)((char *)D_09B6DF08 + 0xFE8);
    *g = func_08A7046C() != 1;
    func_08A71E44(s, k);
}
