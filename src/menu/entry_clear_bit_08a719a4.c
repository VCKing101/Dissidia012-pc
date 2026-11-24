/* Clears the low bit of the flag byte of entry *idx in the 18-byte entry table. */
extern unsigned char D_09B6DF08[];
void func_08A719A4(int a, int b, short *idx) {
    unsigned char *e = D_09B6DF08 + 0xFF0 + *idx * 0x12;
    e[5] = e[5] & ~1;
}
