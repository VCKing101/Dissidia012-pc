/* Sets the two globals at 0x09B6EEF0, runs the setup on self and sets the low bit of the flags word at +4. */
/* Sets the two globals at 0x09B6EEF0, runs the setup on self and sets the low bit of the flags word at +4. */
extern int D_09B6DF08[];
extern void func_08A71E44(int self, int b);
void func_08A71330(int self, int b) {
    int *g = (int *)((char *)D_09B6DF08 + 0xFE8);
    g[0] = 2;
    g[1] = 0;
    func_08A71E44(self, b);
    *(unsigned *)(self + 4) = *(unsigned *)(self + 4) | 1;
}
