/* Calls func_088D9C8C with the float twice when the pointed-to word is non-null. */
extern void func_088D9C8C(float a, float b);
void func_08B418D4(float v, int *p) {
    if (*p != 0) {
        func_088D9C8C(v, v);
    }
}
