/* Sets the low bit of the flag byte of entry *idx in the 18-byte entry table, then runs the follow-up. */
extern unsigned char D_09B6DF08[];
extern void func_08A72120(int a, int b);
void func_08A71960(int a, int b, short *idx) {
    unsigned char *e = D_09B6DF08 + 0xFF0 + *idx * 0x12;
    e[5] = e[5] | 1;
    func_08A72120(a, b);
}
