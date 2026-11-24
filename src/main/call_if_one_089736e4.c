/* If the byte at 0x44 equals 1, calls func_0897381C(self, word at 0, word at 0xC). */
extern void func_0897381C(int *, int, int);
void func_089736E4(int *self) {
    if (((unsigned char *)self)[0x44] == 1) {
        func_0897381C(self, self[0], self[0xC / 4]);
    }
}
