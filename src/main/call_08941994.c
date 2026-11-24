/* Calls func_089446F0 with the word at 0x1C and returns whether it gave non-zero. */
extern int func_089446F0(int, int, int);
int func_08941994(int *self, int b) {
    if (func_089446F0(self[0x1C / 4], b, 0) == 0) { return 0; }
    return 1;
}
