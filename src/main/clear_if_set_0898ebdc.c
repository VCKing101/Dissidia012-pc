/* Clears the word at offset 0x2C if it is set. */
void func_0898EBDC(int *p) {
    int cur = p[11];
    if (!cur) { return; }
    p[11] = 0;
}
